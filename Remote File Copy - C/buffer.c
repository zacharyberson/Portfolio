#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "buffer.h"
#include "packet.h"
#include "lib.h"
#include "cpe464.h"

static uint32_t buf_size;
static uint32_t win_size;
static char* chunk;
static char* buffer;
static uint32_t sbottom;
static uint32_t stop;
static uint32_t cbottom;
static uint32_t ctop;
static FILE* file;

/* gets first no-acked chunk after prev_seq */
int next_noack(uint32_t prev_seq) {
   int i = prev_seq - sbottom;
   
   if(prev_seq == 0)
      return sbottom;
   if(win_size == 1) {
      if(prev_seq == sbottom) {
         stop++;
         return ++sbottom;
      }
      else
         return sbottom;
   }
   while(++i < win_size) {
      if(!ISACK(*(chunk + ((sbottom + i)%win_size)))) {
         return sbottom + i;
      }
   }
   return sbottom + i;
}

/* gets first unused chunk after prev_seq */
int next_unused(uint32_t prev_seq) {
   int i = prev_seq - cbottom;
   
   if(prev_seq == 0)
      return cbottom;
   if(win_size == 1) {
      if(prev_seq == cbottom) {
         ctop++;
         return ++cbottom;
      }
      else
         return cbottom;
   }
   while(++i < win_size) {
      if(!ISUSED(*(chunk + ((cbottom + i)%win_size)))) {
         return cbottom + i;
      }
   }
   return cbottom + i;
}

int init_buffer(uint32_t buffer_size, uint32_t window_size, char* filename) {
   buf_size = buffer_size;
   win_size = window_size;
   buffer = calloc(win_size, buf_size);
   chunk = calloc(win_size, 1);
   cbottom = sbottom = 0;
   ctop = stop = win_size - 1;
   
   if(filename) {
      file = fopen(filename, "w");
      if(NULL == file) {
         printf("Error opening local file: %s.\n", filename);
         exit(INVALID_FILENAME);
      }
   }
   else
      file = NULL;
   
   return 0;
}

/* seq, starting at 0, indicates which packet to
 * write data pointed by data to */
int bwrite(uint32_t seq, char* data) {
   int index = seq % win_size;
   uint32_t sz = 0;
   
   if(win_size == 1) {
      if(file) {
         memcpy(&sz, (data + 7), 4);
         fwrite(data + HEADER_SIZE, 1, sz, file);
         fflush(file);
         *chunk &= !USED;
         cbottom++;
         ctop++;
      }
      else {
         memcpy(buffer, data, buf_size);
         *chunk &= !ACK;
         cbottom++;
         ctop++;
      }
   }
   else {
      if(!buffer) return -1;
      memcpy(buffer + (index * buf_size), data, buf_size);
      *(chunk + index) |= USED;
      if(seq == cbottom) {
         if(file) {
            memcpy(&sz, (buffer + (index * buf_size) + 7), 4);
            fwrite(buffer + (index * buf_size) + HEADER_SIZE, 1, sz, file);
            fflush(file);
         }
         *(chunk + index) &= !USED;
         if(!cbottom) {
            cbottom = 1;
         }
         else
            cbottom = next_unused(cbottom);
         while(++seq < cbottom) {
            memcpy(&sz, (buffer + ((seq % win_size) * buf_size) + 7), 4);
            fwrite(buffer + ((seq % win_size) * buf_size) + HEADER_SIZE, 1,
                    sz, file);
            fflush(file);
            *(chunk + (seq % win_size)) &= !USED;
         }
         ctop = cbottom + win_size - 1;
      }
   }
   return 0;
}

int bread(uint32_t seq, char* result) {
   if(!buffer) return -1;
   
   memcpy(result, buffer + (seq%win_size)*buf_size, buf_size);
   
   return 0;
}

int isUsed(uint32_t seq) {
   return ISUSED(*(chunk + (seq%win_size)));
}

int isAck(uint32_t seq) {
   return ISACK(*(chunk + (seq%win_size)));
}

int mark_ack(uint32_t seq) {   
   if(seq > sbottom) {
      sbottom = seq;
      stop = sbottom + win_size - 1;
   }
   return 0;
}

int destroy_buffer() {
   if(!buffer) return -1;
   
   free(buffer);
   free(chunk);
   buffer = NULL;
   chunk = NULL;
   if(file) {
      fclose(file);
      file = NULL;
   }
   
   return 0;
}


