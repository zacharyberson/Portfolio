#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/select.h>
#include "server.h"
#include "buffer.h"
#include "packet.h"
#include "connection.h"
#include "cpe464.h"
#include "lib.h"

static double error_percent;
static uint16_t port;
static STATE state;
static int socket_listen;
static int socket_talk;
static fd_set parent_set;
static fd_set local_set;
static struct sockaddr_in addr;
static uint32_t buf_size;
static uint32_t win_size;
static BOOLEAN eof = FALSE;
static FILE* local_file = NULL;
static uint32_t last = (0-1);
static uint32_t loaded = 0;
static char* buf = NULL;
static socklen_t sz = 0;
static char temp_buf[HEADER_SIZE + MAX_FILENAME_LEN + 1];
static int filename_size = 0;

static void init() {
   socket_talk = -1;
   buf_size = 0;
   win_size = 0;
   eof = FALSE;
   local_file = NULL;
   last = (0-1);
   loaded = 0;
   buf = NULL;
   sz = sizeof(addr);
}

static void print_usage(char* program_name, char* err) {
   fprintf(stderr, "error: %s\n", err);
   fprintf(stderr, "usage: %s <error %%> [remote port]\n", program_name);
   exit(INVALID_ARGS);
}

static void parse_args(int argc, char** argv) {
   uint32_t templ;
   double tempd;
   
   if(argc < MIN_NUM_SERVER_ARGS || argc > MAX_NUM_SERVER_ARGS)
      print_usage(*argv, "invalid number of args");
      
   tempd = strtod(*(argv + OFFSET_ERROR_PERCENT), NULL);
   if(tempd < MIN_PERCENT_ERROR || tempd > MAX_PERCENT_ERROR)
      print_usage(*argv, "invalid error-percentage");
   else
      error_percent = tempd;
   
   if(argc == MAX_NUM_SERVER_ARGS) {
      templ = (uint32_t)strtol(*(argv + OFFSET_PORT), NULL, 10);
      if(**(argv + OFFSET_PORT) == '-' || templ > MAX_PORT)
         print_usage(*argv, "invalid port");
      else
         port = (uint16_t)templ;
   }
   else
      port = 0;
   
   sendErr_init(error_percent, DROP_ON, FLIP_ON, DEBUG_ON, RSEED_OFF);
}

static void clean_state() {
   if(local_file) {
      fclose(local_file);
   }
   close(socket_talk);
   destroy_buffer();
   if(buf)
      free(buf);
   exit(0);
}

static void reap_zombies() {
   while(waitpid(-1, NULL, WNOHANG) > 0) ;
}

static void buffer_packet() {
   filename_size = recvfrom(socket_listen, temp_buf,
                            MAX_FILENAME_LEN + HEADER_SIZE, 0,
                            (struct sockaddr*)(&addr), &sz) - HEADER_SIZE;
}
static void wait_for_connect() {
   int isParent = TRUE;

   FD_ZERO(&parent_set);
   FD_SET(socket_listen, &parent_set);
   selectMod(socket_listen + 1, &parent_set, NULL, NULL, NULL);
   while(isParent) {
      memset(temp_buf, 0, HEADER_SIZE + MAX_FILENAME_LEN + 1);
      FD_ZERO(&parent_set);
      FD_SET(socket_listen, &parent_set);
      selectMod(socket_listen + 1, &parent_set, NULL, NULL, NULL);
      buffer_packet();
      reap_zombies();
      isParent = fork();
   }
   state = RECV_FILENAME;
}

static void recv_filename() {
   packet_header head;
 
   socket_talk = get_socket(0, FALSE);
   FD_ZERO(&local_set);
   FD_SET(socket_talk, &local_set);
                            
   if(filename_size + HEADER_SIZE <= 0) {
      exit(RECV_FILENAME_ERR);
   }
   
   if(0 != in_cksum((unsigned short*)temp_buf, HEADER_SIZE + filename_size)) {
      clean_state();
      return;
   }
   
   local_file = fopen(temp_buf + HEADER_SIZE, "rb");
   if(!local_file){
      head.flag = INVALID_FILENAME;
      memset(temp_buf, 0, HEADER_SIZE + MAX_FILENAME_LEN + 1);
      memcpy(temp_buf, &head, HEADER_SIZE);
      write_checksum(temp_buf, HEADER_SIZE);
      sendtoErr(socket_talk, temp_buf, HEADER_SIZE, 0,
                (struct sockaddr*)&addr, sz);
      clean_state();
      return;
   }
   get_header(&head, temp_buf);
   buf_size = head.data_size;
   win_size = head.sequence;
   init_buffer(head.data_size + HEADER_SIZE, head.sequence, NULL);
   buf = calloc(HEADER_SIZE + buf_size + 1, 1);
   
   head.sequence = 0;
   head.checksum = 0;
   head.flag = FLAG_FILENAME_ACK;
   head.data_size = 0;
   memset(temp_buf, 0, HEADER_SIZE + MAX_FILENAME_LEN + 1);
   memcpy(temp_buf, &head, HEADER_SIZE);
   write_checksum(temp_buf, HEADER_SIZE);
   sendtoErr(socket_talk, temp_buf, HEADER_SIZE, 0,
    (struct sockaddr*)&addr, sz);
   
   state = SEND_DATA;
   return;
}

static void send_packet(int sequence) {
   int size = HEADER_SIZE + buf_size;
   
   bread(sequence, buf);
   sendtoErr(socket_talk, buf, size, 0, (struct sockaddr*)&addr, sz);
}

static RESPONSE check_response(int sec) {
   struct timeval time;
   int num;
   packet_header head;
   RESPONSE res = -1;
   
   time.tv_sec = sec;
   time.tv_usec = 0;
   FD_ZERO(&local_set);
   FD_SET(socket_talk, &local_set);
   num = selectMod(socket_talk + 1, &local_set, NULL, NULL, &time);
   
   if(num) {
      recvfrom(socket_talk, buf, HEADER_SIZE, 0,
       (struct sockaddr*)(&addr), &sz);
      if(!in_cksum((unsigned short*)buf, HEADER_SIZE)) {
         get_header(&head, buf);
         if(FLAG_RR == head.flag) {
            res = RESP_RR;
            mark_ack(head.sequence);
         }
         else if(FLAG_SREJ == head.flag) {
            res = RESP_SREJ;
            send_packet(head.sequence);
         }
         else if(FLAG_EOF_ACK == head.flag) {
            res = RESP_EOF;
            state = DONE;
         }
      }
   }
   
   return res;
}

static void send_single_packet() {
   struct timeval time;
   int num = 0;
   int retry = 0;
   RESPONSE resp = -1;
   
   while(retry < 10) {
      time.tv_sec = 1;
      time.tv_usec = 0;
      FD_ZERO(&local_set);
      FD_SET(socket_talk, &local_set);
      
      num = selectMod(socket_talk + 1, &local_set, NULL, NULL, &time);
      if(num) {
         resp = check_response(0);
         if((!eof && (RESP_RR == resp)) || (RESP_SREJ == resp)) {
            state = SEND_DATA;
            break;
         }
         else if(eof && (RESP_RR == resp) && (next_noack(0) == last)) {
            state = SEND_EOF;
            break;
         }
      }
      else {
         retry++;
         send_packet(0);
      }
   }
   if(retry == 10)
      state = DONE;
}

static void send_data() {
   int size = HEADER_SIZE + buf_size;
   int seq = next_noack(0);
   int top = seq + win_size;
   packet_header head;
   
   //buffer window to send
   while((loaded < top) && !eof) {
      head.sequence = loaded;
      head.checksum = 0;
      head.flag = FLAG_DATA;
      memset(buf, 0, size + 1);
      head.data_size = fread(buf + HEADER_SIZE, 1, buf_size, local_file);
      
      if(head.data_size) {
         memset(buf, 0, HEADER_SIZE);
         memcpy(buf, &head, HEADER_SIZE);
         write_checksum(buf, HEADER_SIZE + head.data_size);
         
         bwrite(loaded, buf);
         memset(buf, 0, size + 1);
         loaded++;
      }
      else if(feof(local_file)) {
         last = loaded;
         eof = TRUE;
         if(1 == last) {
            send_packet(0);
            send_single_packet();
            return;
         }
      }
   }
   while(seq < loaded) {
      send_packet(seq);
      seq++;
      check_response(0);
   }
   if(eof && (next_noack(0) == last)) {
      state = SEND_EOF;
   }
   else {
      state = WAIT_ACK;
   }
   return;
}

static void wait_ack() {
   struct timeval time;
   int num = 0;
   int retry = 0;
   RESPONSE resp = -1;
   
   while(retry < 10) {
      time.tv_sec = 1;
      time.tv_usec = 0;
      FD_ZERO(&local_set);
      FD_SET(socket_talk, &local_set);
      
      num = selectMod(socket_talk + 1, &local_set, NULL, NULL, &time);
      if(num) {
         resp = check_response(0);
         if((!eof && (RESP_RR == resp)) || (RESP_SREJ == resp)) {
            state = SEND_DATA;
            break;
         }
         else if(eof && (RESP_RR == resp) && (next_noack(0) == last)) {
            state = SEND_EOF;
            break;
         }
      }
      else {
         retry++;
         send_packet(next_noack(0));
      }
   }
   
   if(eof && (RESP_RR == resp) && (next_noack(0) == last)) {
      state = SEND_EOF;
   }
   
   if(10 == retry) {
      state = DONE;
   }
      
   return;
}

static void send_eof() {
   int retry = 0;
   RESPONSE resp = -1;
   int size = HEADER_SIZE;
   packet_header head;
   
   head.sequence = 0;
   head.checksum = 0;
   head.flag = FLAG_EOF;
   head.data_size = 0;
   memset(buf, 0, HEADER_SIZE + 1);
   memcpy(buf, &head, HEADER_SIZE);
   write_checksum(buf, HEADER_SIZE);
   sendtoErr(socket_talk, buf, size, 0, (struct sockaddr*)&addr, sz);
   
   while(retry < 10) {
      resp = check_response(1);
      if(RESP_EOF == resp) {
         state = DONE;
         break;
      }
      else if(-1 == resp)
         retry++;
      head.sequence = 0;
      head.checksum = 0;
      head.flag = FLAG_EOF;
      head.data_size = 0;
      memset(buf, 0, HEADER_SIZE + 1);
      memcpy(buf, &head, HEADER_SIZE);
      write_checksum(buf, HEADER_SIZE);
      sendtoErr(socket_talk, buf, size, 0, (struct sockaddr*)&addr, sz);
   }
   state = DONE;
   return;
}

static void run() {
   while(TRUE) {
      switch (state) {
         case INIT:
            init();
            wait_for_connect();
            break;
         case RECV_FILENAME:
            recv_filename();
            break;
         case SEND_DATA:
            send_data();
            break;
         case WAIT_ACK:
            wait_ack();
            break;
         case SEND_EOF:
            send_eof();
            break;
         case DONE:
            clean_state();
            break;
         default:
            exit(INVALID_STATE);
      }
   }
}

int main(int argc, char** argv) {
   parse_args(argc, argv);
   socket_listen = get_socket(port, TRUE);
   state = INIT;
   run();
   return 0;
}
