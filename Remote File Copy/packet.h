#ifndef __PACKET_H
#define __PACKET_H

#include <stdint.h>

#define HEADER_SIZE 11

/* for filename headers, data_size is the buffer size
   to be used for data in subsequent data packets
   for everything else, it is the size of the data
   for filename headers, sequence is window size for
   subsequent packets */
   
typedef struct {
   uint32_t sequence;
   uint16_t checksum;
   uint8_t flag;
   uint32_t data_size;
}__attribute__((packed, aligned(1))) packet_header;

void print_header(void* header);
int get_header(packet_header* head, char* packet);
int get_data(char* result, char* packet, uint32_t size);
int write_checksum(void* buffer, int size);


#endif