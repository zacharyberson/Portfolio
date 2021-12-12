#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "lib.h"
#include "error_codes.h"
#include "bitwise.h"

int set_bit(char* block, int index) {
   uint8_t* pos = (uint8_t*)block;
   uint8_t val, mask;

   if(NULL == pos) {
      return INVALID_POINTER;
   }
   if((BLOCKSIZE * 8) <= index) {
      return INVALID_INDEX;
   }

//fprintf(stderr, "   setting bit in index: %d\n", index);
   pos += (index / 8);
   val = *(pos);
//fprintf(stderr, "   initial value: %02x\n", val);
   
   mask = 0x80 >> (index % 8);
//fprintf(stderr, "   using mask: %02x\n", mask);
   val |= mask;
//fprintf(stderr, "   now the value: %02x\n", val);
   *pos = val;

   return 0;
}

int clr_bit(char* block, int index) {
   uint8_t* pos = (uint8_t*)block;
   uint8_t val, mask;

   if(NULL == pos) {
      return INVALID_POINTER;
   }
   if((BLOCKSIZE * 8) <= index) {
      return INVALID_INDEX;
   }

   pos += (index / 8);
   val = *(pos);
   
   mask = ~(0x80 >> (index % 8));
   val &= mask;
   *pos = val;

   return 0;
}

int chk_bit(char* block, int index) {
   uint8_t* pos = (uint8_t*)block;
   uint8_t val, mask;

   if(NULL == pos) {
      return INVALID_POINTER;
   }
   if((BLOCKSIZE * 8) <= index) {
      return INVALID_INDEX;
   }

   pos += (index / 8);
   val = *(pos);
   
   mask = 0x80 >> (index % 8);
   return val & mask;
}
