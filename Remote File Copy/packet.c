#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "packet.h"
#include "cpe464.h"

int get_header(packet_header* head, char* packet) {
   memcpy(head, packet, HEADER_SIZE);
   return 0;
}

int get_data(char* result, char* packet, uint32_t size) {
   memcpy(result, packet + HEADER_SIZE, size);
   return 0;
}

int write_checksum(void* buffer, int size) {
   unsigned short* temp = (unsigned short*)buffer;
   
   temp[2] = 0;
   temp[2] = in_cksum(temp, size);

   return 0;
}
