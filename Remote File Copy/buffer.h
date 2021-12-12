#ifndef __BUFFER_H
#define __BUFFER_H

#include <stdint.h>

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define ISACK(a) (((a)&2)>>1)
#define ISUSED(a) ((a)&1)
#define ACK 2
#define USED 1

void print_buffer();

int init_buffer(uint32_t buffer_size, uint32_t window_size, char* filename);


/* server and client side */
/* seq, starting at 0, indicates which packet to
 *  write data pointed by data to */
int bwrite(uint32_t seq, char* data);
int bread(uint32_t seq, char* result);
int destroy_buffer();



/* client side */
int isUsed(uint32_t seq);
// prev_seq == 0, return cbottom
int next_unused(uint32_t prev_seq);



/* server side */
int mark_ack(uint32_t seq);
int isAck(uint32_t seq);
// if prev_seq == 0, return sbottom
int next_noack(uint32_t prev_seq);


#endif
