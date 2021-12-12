#ifndef __LIB_H
#define __LIB_H

#define MIN(a,b) ((a)<(b)?(a):(b))

typedef enum {INVALID, OPENR, OPENW, OPENRW, READ, WRITE, CLOSE} COMMAND;
typedef enum {RW, R, W} READWRITE;

#define BLOCKSIZE 4096
#define DATASIZE 4070
#define BLOCK_BITMAP 0
#define BLOCK_FILES 1
#define SIZE_NAME 20

#endif
