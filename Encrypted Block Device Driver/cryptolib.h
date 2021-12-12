/* 
   provides simple file system and encryption
   global BITMAP indicates which blocks are currently used
   gloabl FILES indicates which blocks contain the start of files
*/


#ifndef __CRYPTOLIB_H
#define __CRYPTOLIB_H

#include <stdint.h>

#include "lib.h"

typedef struct BLOCK {
   char name[SIZE_NAME];
   uint32_t size;
   uint16_t next;
   char data[DATASIZE];
} BLOCK;

int c_open_disk(char* filename, int nBytes, READWRITE rw);

int c_close_disk();

/* outputs data from filesystem from file named filename into outfile
   filename must not be longer than 7 characters in length
*/
int c_read(char* filename, char* outfile);

/* writes data into filesystem in file named filename
   filename must not be longer than 7 characters in length
*/
int c_write(char* data, int size, char* filename);

#endif
