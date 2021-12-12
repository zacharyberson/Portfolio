//#include <limits.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/select.h>
#include <unistd.h>
#include "cpe464.h"
#include "rcopy.h"
#include "lib.h"
#include "buffer.h"
#include "packet.h"
#include "connection.h"

static int sock;
static uint32_t buffer;
static double error_percent;
static uint32_t window_size;
static char* local_file;
static char* remote_file;
static struct sockaddr addr;
static socklen_t addr_len;
static STATE state;

static void print_usage(char* program_name, char* err) {
   fprintf(stderr, "error: %s\n", err);
   fprintf(stderr, "usage: %s <local file> <remote-file> ", program_name);
   fprintf(stderr, "<buffer> <error %%> <window size> <remote machine> ");
   fprintf(stderr, "<remote port>\n");
   exit(INVALID_ARGS);
}

int fill_addr(char* remote_machine, uint16_t remote_port) {
   struct sockaddr_in* temp = (struct sockaddr_in*)&addr;
   
   temp->sin_family = AF_INET;
   temp->sin_port = htons(remote_port);
   dns_lookup(remote_machine, &(temp->sin_addr));
   
   memset(temp->sin_zero, 0, 8);
   
   return 0;
}

static void parse_args(int argc, char** argv) {
   uint32_t templ;
   double tempd;
   char* remote_machine;
   uint16_t remote_port;
   
   if(NUM_RCOPY_ARGS != argc)
      print_usage(*argv, "invalid number of args");
   
   local_file = *(argv + OFFSET_LOCAL_FILE);
   if(strlen(local_file) > MAX_FILENAME_LEN)
      print_usage(*argv, "local filename exceeds 100 char");
   
   remote_file = *(argv + OFFSET_REMOTE_FILE);
   if(strlen(remote_file) > MAX_FILENAME_LEN)
      print_usage(*argv, "remote filename exceeds 100 char");
   
   templ = (uint32_t)strtol(*(argv + OFFSET_BUFFER_SIZE), NULL, 10);
   if(**(argv + OFFSET_BUFFER_SIZE) == '-' || templ == 0)
      print_usage(*argv, "invalid buffer");
   else
      buffer = templ;
   
   tempd = strtod(*(argv + OFFSET_ERROR_PERCENT), NULL);
   if(tempd < MIN_PERCENT_ERROR || tempd > MAX_PERCENT_ERROR)
      print_usage(*argv, "invalid error-percentage");
   else
      error_percent = tempd;
   
   templ = (uint32_t)strtol(*(argv + OFFSET_WINDOW_SIZE), NULL, 10);
   if(**(argv + OFFSET_WINDOW_SIZE) == '-' || templ == 0)
      print_usage(*argv, "invalid window size");
   else
      window_size = templ;
   
   remote_machine = *(argv + OFFSET_REMOTE_MACHINE);
   
   templ = (uint32_t)strtol(*(argv + OFFSET_REMOTE_PORT), NULL, 10);
   if(**(argv + OFFSET_REMOTE_PORT) == '-' || templ > MAX_PORT)
      print_usage(*argv, "invalid port");
   else
      remote_port = (uint16_t)templ;
   
   fill_addr(remote_machine, remote_port);
}

int open_connection() {
   if(0 > (sock = get_socket(0, FALSE))) exit(-1);
   state = SEND_FILENAME;
   return 0;
}

int send_filename() {
   char buf[HEADER_SIZE + 1 + MAX_FILENAME_LEN] = {0};
   char recv[HEADER_SIZE + 1] = {0};
   packet_header head;
   struct timeval time;
   int retry = 0;
   fd_set set;
   
   head.sequence = window_size;
   head.checksum = 0;
   head.flag = FLAG_FILENAME;
   head.data_size = buffer;
   
   memcpy(buf, &head, HEADER_SIZE);
   memcpy(buf + HEADER_SIZE, remote_file, strlen(remote_file));
   
   write_checksum(buf, HEADER_SIZE + strlen(remote_file));
   
   sendtoErr(sock, buf, HEADER_SIZE + strlen(remote_file), 0,
              &addr, sizeof(addr));
   while(retry < 10) {
      FD_ZERO(&set);
      FD_SET(sock, &set);
      time.tv_sec = 1;
      time.tv_usec = 0;
      selectMod(sock + 1, &set, NULL, NULL, &time);
      if(FD_ISSET(sock, &set)) {
         recvfrom(sock, recv, HEADER_SIZE, MSG_PEEK, &addr, &addr_len);
         if(!in_cksum((unsigned short*)recv, HEADER_SIZE + 1)) {
            get_header(&head, recv);
            if(FLAG_FILENAME_ACK == head.flag) {
               printf("Server successfully opened %s on server.\n",
                       remote_file);
               recvfrom(sock, recv, HEADER_SIZE, 0, &addr, &addr_len);
               state = DATA;
               return 0;
            }
            else if(FLAG_INVALID_FILENAME == head.flag) {
               printf("Error during file open of %s on server.\n", remote_file);
               recvfrom(sock, recv, HEADER_SIZE, 0, &addr, &addr_len);
               state = EXIT;
               return 0;
            }
            else if(FLAG_DATA == head.flag) {
               state = DATA;
               return 0;
            }
            else {
               exit(INVALID_STATE);
            }
         }
         else {
            recvfrom(sock, recv, HEADER_SIZE, 0, &addr, &addr_len);
         }
      }
      else {
         retry++;
         sendtoErr(sock, buf, HEADER_SIZE + strlen(remote_file), 0,
                    &addr, sizeof(addr));
      }
   }
   
   state = EXIT;
   return 0;
}

int send_response(uint32_t seq, RESPONSE res) {
   char* send = calloc(HEADER_SIZE + 1, 1);
   packet_header head;
   
   head.sequence = seq;
   head.checksum = 0;
   if(res == RESP_SREJ) {
      head.flag = FLAG_SREJ;
   }
   else if(res == RESP_RR) {
      head.flag = FLAG_RR;
   }
   else if(res == RESP_EOF) {
      head.flag = FLAG_EOF_ACK;
   }
   else {
      exit(INVALID_CALL);
   }
   head.data_size = 0;
   memset(send, 0, HEADER_SIZE + 1);
   memcpy(send, &head, HEADER_SIZE);
   write_checksum(send, HEADER_SIZE);
   
   sendtoErr(sock, send, HEADER_SIZE, 0, &addr, sizeof(addr));
   free(send);
   return 0;
}

int recv_data() {
   packet_header head;
   char* recv;
   RESPONSE res;
   int retry = 0;
   fd_set set;
   struct timeval time;
   
   recv = calloc(HEADER_SIZE + buffer + 1, 1);
   
   while((retry != 10) && (head.flag != FLAG_EOF)) {
      retry = 0;
      while(retry < 10) {
         FD_ZERO(&set);
         FD_SET(sock, &set);
         time.tv_sec = 1;
         time.tv_usec = 0;
         selectMod(sock + 1, &set, NULL, NULL, &time);
         if(FD_ISSET(sock, &set)) {
            memset(recv, 0, HEADER_SIZE + buffer + 1);
            recvfrom(sock, recv, HEADER_SIZE + buffer, 0, &addr, &addr_len);
            if(!in_cksum((unsigned short*)recv, HEADER_SIZE + buffer)) {
               retry = 0;
               get_header(&head, recv);
               if(FLAG_DATA == head.flag) {
                  if(head.sequence > next_unused(0)) {
                     res = RESP_SREJ;
                     if(!isUsed(head.sequence)) {
                        bwrite(head.sequence, recv);
                     }
                  }
                  else if(head.sequence == next_unused(0)) {
                     res = RESP_RR;
                     bwrite(head.sequence, recv);
                  }
                  else {
                     res = RESP_RR;
                  }
                  send_response(next_unused(0), res);
               }
               else if(FLAG_EOF == head.flag) {
                  send_response(0, RESP_EOF);
                  state = EXIT;
                  free(recv);
                  return 0;
               }
               else {
                  exit(INVALID_STATE);
               }
            }
         }
         else {
            retry++;
            send_response(next_unused(0), RESP_RR);
         }
      }
   }
   
   state = EXIT;
   free(recv);
   return 0;
}

int exit_prog() {
   destroy_buffer();
   exit(0);
   return 0;
}

static void run() {
   while(TRUE) {
      switch (state) {
         case INIT:
            open_connection();
            break;
         case SEND_FILENAME:
            send_filename();
            break;
         case DATA:
            recv_data();
            break;
         case EXIT:
            exit_prog();
            break;
         default:
            exit(INVALID_STATE);
      }
   }
}

static void init(int argc, char** argv) {
   parse_args(argc, argv);
   init_buffer(buffer + HEADER_SIZE, window_size, local_file);
   sendErr_init(error_percent, DROP_ON, FLIP_ON, DEBUG_ON, RSEED_OFF);
//   print_buffer();
   state = INIT;
}

int main(int argc, char** argv) {
   init(argc, argv);
   
   run();
   
   return 0;
}
