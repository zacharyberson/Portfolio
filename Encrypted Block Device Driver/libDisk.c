#include <stdio.h>
#include <string.h>

#include "lib.h"
#include "libDisk.h"
#include "error_codes.h"

static Disk *disk_list = NULL;

static int createDisk(char *filename, int nBytes, FILE *fd) {
   Disk *add = calloc(1, sizeof(Disk));

   if(NULL != disk_list) {
      free(add);
      return ALREADY_MOUNTED;
   }
   add->size = nBytes;
   add->name = calloc(strlen(filename) + 1, sizeof(char));
   strcpy(add->name, filename);
   add->file = fd;

   disk_list = add;

   return 1;
}

int openDisk(char *filename, int nBytes) {
   FILE *fd = NULL;
   int disk_num;
     
   if (nBytes > 0) {
      fd = fopen(filename, "r+b");
   }
   else {
      fd = fopen(filename, "rb");    
   }
   if(!fd) {
      fprintf(stderr, "error: cannot open file \"%s\"\n", filename);
      return INVALID_FILENAME;
   }
   disk_num = createDisk(filename, nBytes, fd); 
   //printf("Open success!\n");
   return disk_num;
}

int readBlock(int bNum, void *block) {
   Disk *temp = disk_list;
   FILE *fd = NULL;
   int read = 0;

//fprintf(stderr, "Reading Block %d\n", bNum);
   if (temp == NULL) {
      printf("Failed here!\n");
      return OPEN_FAILURE;
   }

   fd = temp->file;
//fprintf(stderr, "temp->file: %li\n", (long)temp->file);
   if (fseek(fd, bNum * BLOCKSIZE, SEEK_SET) == 0) {
      
//   fprintf(stderr, "Seeked \n");
      read = fread(block, BLOCKSIZE, 1, fd);
//fprintf(stderr, "Read %d elements from disk \n", read);
      if (read > 0)
         return 0;
      else
         perror("read nothing");
   }
   return READ_ERROR;
}

int writeBlock(int bNum, void *block){
   Disk *temp;
   FILE *fd;
   int write;

   if ((temp = disk_list) == NULL)
      return OPEN_FAILURE;

   fd = temp->file;
//fprintf(stderr, "temp->file: %li\n", (long)temp->file);
   if (fseek(fd, bNum * BLOCKSIZE, SEEK_SET) == 0) {
      write = fwrite(block, BLOCKSIZE, 1, fd);
      if (write > 0) {
         return 0;
      }
   }
   return WRITE_ERROR;
}

void closeDisk() {
   Disk *temp;

   if ((temp = disk_list) == NULL) {
      printf("Invalid disk to close\n");
      return;
   }
   if (fflush(temp->file) != 0)
      printf("Flushed no data\n");

   fclose(temp->file);
   free(disk_list->name);
   free(disk_list);
   disk_list = NULL;
}

int get_name(char* buff) {
   int i = 0;
   char* name;
   
//   fprintf(stderr, "getting disk name\n");
   if(disk_list == NULL) {
      return 0;
   }
   name = disk_list->name;
   while (name[i] != '\0') {
      buff[i] = name[i];
   }
   return 1;
}

int getSize() {
   Disk *disk = disk_list;

//   fprintf(stderr, "getting disk size\n");
   if (NULL == disk) {
      fprintf(stderr, "error: no disk mounted\n");
      return -1;
   }
   return disk->size;
}

/*
static int create_block()
static int get_num_blocks()
*/
