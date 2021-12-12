/*
   Note: Does not catch invalid sized blocks, that would require setting up
   signal handler to avoid segfaults.
*/

#ifndef __BITWISE_H
#define __BITWISE_H

#include <stdint.h>

/*
   Sets the bit at index of block to 1
      block = pointer to block with togglable bits
      index = index of bit to set to 1
         Values of index: 0 <= index < BLOCKSIZE
   Returns
      0 on success
      error code indicated by error_codes.h on error
*/
int set_bit(char* block, int index);
int clr_bit(char* block, int index);
int chk_bit(char* block, int index);


#endif
