#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "cryptolib.h"
#include "lib.h"
#include "error_codes.h"
#include "bitwise.h"
#include "libDisk.h"

static int import_data(char *datafile, char **data) {
   int sz, c;
   int i = 0;
   FILE *fp;
   
   fp = fopen(datafile, "rb");
   if(!fp) {
      fprintf(stderr, "Can't open %s\n", datafile);
      exit(NO_SUCH_FILE);
   }
   fseek(fp, 0L, SEEK_END);
   sz = ftell(fp);
   rewind(fp);
   *data = calloc(sz + 1, 1);
   while((c = fgetc(fp)) != EOF) {
      (*data)[i++] = (char) c;
   }
//fprintf(stderr, "data:\n%s\n", *data);
   fclose(fp);
   
   return sz;
}

static char* m_getline(char *buf, int size, FILE* fp) {
   int len;
   char *ret;
   
   ret = fgets(buf, size, fp);
   if(!ret) {
      return NULL;
   }
   len = strlen(buf);
   buf[len-1] = '\0';
   
   return buf;
}

static COMMAND get_command(FILE* fp) {
   char command[10] = {0};
   
   assert(m_getline(command, 10, fp));
   if(!strcmp("openr", command)) {
      return OPENR;
   }
   if(!strcmp("openw", command)) {
      return OPENW;
   }
   if(!strcmp("openrw", command)) {
      return OPENRW;
   }
   if(!strcmp("read", command)) {
      return READ;
   }
   if(!strcmp("write", command)) {
      return WRITE;
   }
   if(!strcmp("close", command)) {
      return CLOSE;
   }
   return INVALID;
}

static void run(char *script) {
   int lines, size;
   COMMAND com;
   FILE *fd = fopen(script, "r");
   char *data = NULL;
   char num[3] = {0};
   char name[SIZE_NAME] = {0};
   char output[20] = {0};
   char datafile[20] = {0};
   
   
   while(NULL != m_getline(num, 3, fd)) {
//      fprintf(stderr, "Press enter to continue...\n");
//      while('\n' != getchar());
      if(!(lines = strtol(num, NULL, 10))) {
         fprintf(stderr, "error: invalid line count in script\n");
         exit(INVALID_FORMAT);
      }
      com = get_command(fd);
      switch(com) {
         case OPENR:
            c_open_disk("vDisk", 0, R);
            break;
         case OPENW:
            c_open_disk("vDisk", 16777216, W);
            break;
         case OPENRW:
            c_open_disk("vDisk", 16777216, RW);
            break;
         case READ:
            m_getline(name, SIZE_NAME, fd);
//fprintf(stderr, "filename: %s\n", name);
            assert(0 < (sprintf(output, ".out%s", name)));
            c_read(name, output);
            break;
         case WRITE:
            m_getline(name, SIZE_NAME, fd);
//fprintf(stderr, "filename: %s\n", name);
            m_getline(datafile, 20, fd);
//fprintf(stderr, "datafile: %s\n", datafile);
            size = import_data(datafile, &data);
            c_write(data, size, name);
            free(data);
            data = NULL;
            break;
         case CLOSE:
            c_close_disk();
            break;
         case INVALID:
            fprintf(stderr, "Invalid case\n");
            exit(INVALID_CASE);
            break;
         default:
            fprintf(stderr, "Invalid switch case\n");
            exit(INVALID_CASE);
      }
      memset(num, 0, 3);
      memset(name, 0, SIZE_NAME);
      memset(output, 0, 20);
      memset(datafile, 0, 20);
   }
}

static void parse_args(int argc, char** argv) {
   if(2 != argc) {
      fprintf(stderr, "usage: %s <script>\n", *argv);
      exit(INVALID_ARGS);
   }
   run(*(argv + 1));
}

int main(int argc, char **argv) {
   parse_args(argc, argv);

   return 0;
}
