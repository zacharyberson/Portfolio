#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "libDisk.h"
#include "cryptolib.h"
#include "bitwise.h"
#include "error_codes.h"

static int createBitmap(READWRITE rw);
static int createFilemap(READWRITE rw);
static int nextFree(int after);
static int nextFile(int after);
static int getFreeBlocks();
static int search(char *filename);
static void write_output(char *data, int size, char *filename);
static void print_filemap();
static void print_bitmap();
static void print_BLOCK(BLOCK *block);


static char* bitmap;
static char* files;

int c_open_disk(char* filename, int nBytes, READWRITE rw) {
   
   if(ALREADY_MOUNTED == openDisk(filename, nBytes)) {
      fprintf(stderr, "can't create disk, unmount current disk first\n");
      return ALREADY_MOUNTED;
   }
   
   if(!nBytes) rw = R;
   
   createBitmap(rw);
   createFilemap(rw);
   return 0;
}

int c_close_disk() {
   if(bitmap) {
      writeBlock(BLOCK_BITMAP, bitmap);
      free(bitmap);
   }
   if(files) {
      writeBlock(BLOCK_FILES, files);
      free(files);
   }
   bitmap = files = NULL;
   closeDisk();
   return 0;
}

int c_read(char* filename, char* outfile) {
   BLOCK block;
   char *data, *pos, first;
   int tsize;
   
   assert (sizeof(block) == BLOCKSIZE);
   
   first = search(filename);
   
   if(NO_SUCH_FILE == first){
      //fprintf(stderr, "File %s does not exist\n", filename);
      return NO_SUCH_FILE;
   }
   
//fprintf(stderr, "reading first fileblock\n");
   readBlock(first, &block);
//fprintf(stderr, "first fileblock size: %u\n", block.size);
   tsize = block.size;
   data = malloc(tsize);
   pos = data;
   memcpy(pos, block.data, MIN(tsize, DATASIZE));
   pos += MIN(tsize, DATASIZE);
   tsize -= MIN(tsize, DATASIZE);
   while(block.next) {
      readBlock(block.next, &block);
      if(block.next) {
         memcpy(pos, block.data, DATASIZE);
         tsize -= DATASIZE;
      }
      else {
         memcpy(pos, block.data, tsize);
         tsize = 0;
      }
      pos += DATASIZE;
   }
//fprintf(stderr, "out data:\n%s\n", data);
   write_output(data, block.size, outfile);
   
   free(data);
   return 0;
}

int c_write(char* data, int size, char* filename) {
   int tsize = size;
   int curr, i, num;
   char* pos = data;
   BLOCK block;
   
   curr = nextFree(BLOCK_FILES);
   if(STORAGE_EXCEEDED == curr) {
      fprintf(stderr, "Can't write file %s: not enough space.\n", filename);
      return STORAGE_EXCEEDED;
   }   
   
   num = (size / DATASIZE) + 1;
   if (num > getFreeBlocks()) {
      fprintf(stderr, "error: not enough space for file %s\n", filename);
      return STORAGE_EXCEEDED;
   }
   
   set_bit(files, curr);
   //for each chunk of data, find and write block
   for(i = 0; i < num; i++) {
//fprintf(stderr, "writing to block: %u with name: %s\n", curr, filename);
      set_bit(bitmap, curr);
      memcpy(block.name, filename, SIZE_NAME);
      block.size = size;
      block.next = nextFree(curr);
      if(i == (num - 1)) {
         block.next = 0;
      }
      memcpy(block.data, pos, MIN(DATASIZE, tsize));
      tsize -= MIN(DATASIZE, tsize);
      pos += DATASIZE;
//      print_BLOCK(&block);
      assert(!writeBlock(curr, &block));
      curr = block.next;      
   }
   
   return 0;
}

static int createBitmap(READWRITE rw) {
   bitmap = calloc(BLOCKSIZE, 1);
   
   if(W != rw) {
      readBlock(BLOCK_BITMAP, bitmap);
   }
   else if(W == rw) {
      set_bit(bitmap, BLOCK_BITMAP);
      set_bit(bitmap, BLOCK_FILES);
      writeBlock(BLOCK_BITMAP, bitmap);
   }
//print_bitmap(0);
   return 0;
}

static int createFilemap(READWRITE rw) {
   files = calloc(BLOCKSIZE, 1);
   
   if(W != rw) {
      readBlock(BLOCK_FILES, files);
   }
   else if(W == rw) {
      set_bit(files, BLOCK_BITMAP);
      set_bit(files, BLOCK_FILES);
      writeBlock(BLOCK_FILES, files);
   }
//print_filemap(0);
   return 0;
}

static int nextFree(int after) {
   int i;
   
   for(i = after + 1; i < BLOCKSIZE * 8; i++) {
      if(!chk_bit(bitmap, i)) return i;
   }
   
   return STORAGE_EXCEEDED;
}

static int nextFile(int after) {
   int i;
   
   for(i = after + 1; i < BLOCKSIZE * 8; i++) {
      if(chk_bit(files, i)) return i;
   }
   
   return NO_MORE_FILES;
}

static int getFreeBlocks() {
   int i;
   int b = 0;
   
   for(i = 2; i < BLOCKSIZE * 8; i++) {
      if(!chk_bit(bitmap, i)) b++;
   }
   
   return b;
}

static int search(char *filename) {
   int next = nextFile(BLOCK_FILES);
   BLOCK block;
   
//fprintf(stderr, "searching for file %s\nstarting at block %u", filename, next);
   while(NO_MORE_FILES != next && next < (BLOCKSIZE * 8)) {
      if(chk_bit(files, next)) {
         readBlock(next, &block);
//print_BLOCK(&block);
//fprintf(stderr, "potential name: %s\n", block.name);
         if(!memcmp(filename, block.name, SIZE_NAME)) {
//fprintf(stderr, "found first fileblock: %u\n", next);
            return next;
         }
      }
      next = nextFile(next);
   }
   
   fprintf(stderr, "file not found\n");
   return NO_SUCH_FILE;
}

static void write_output(char *data, int size, char *filename) {
   FILE *fd = fopen(filename, "w");
   
   fwrite(data, 1, size, fd);
   fclose(fd);
}

static void print_bitmap(int n) {
   int i;
   char file[25] = {0};
   FILE *fp;
   char format[5] = {0};

   sprintf(file, "bitmap%u.debug", n);
   fp = fopen(file, "wb");
   for(i = 0; i < BLOCKSIZE; i++) {
      if(!(i%8)) {
         fwrite("\n", 1, 1, fp);
      }
      fwrite(" ", 1, 1, fp);
      sprintf(format, "%02x", *((unsigned char*)(bitmap + i)));
      fwrite(format, 2, 1, fp);
   }
}

static void print_filemap(int n) {
   int i;
   char file[25] = {0};
   FILE *fp;
   char format[5] = {0};

   sprintf(file, "filemap%u.debug", n);
   fp = fopen(file, "wb");
   for(i = 0; i < BLOCKSIZE; i++) {
      if(!(i%8)) {
         fwrite("\n", 1, 1, fp);
      }
      fwrite(" ", 1, 1, fp);
      sprintf(format, "%02x", *((unsigned char*)(files + i)));
      fwrite(format, 2, 1, fp);
   }
}

static void print_BLOCK(BLOCK *block) {
   fprintf(stderr, "---BLOCK---\n");
//   name: %s\n", block->name);
   fprintf(stderr, "   size: %u\n", block->size);
   fprintf(stderr, "   next: %u\n", block->next);
   fprintf(stderr, "-----------\n");
}
