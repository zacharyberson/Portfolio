#ifndef __SERVER_H
#define __SERVER_H

#define MAX_NUM_SERVER_ARGS 3
#define MIN_NUM_SERVER_ARGS 2
#define OFFSET_ERROR_PERCENT 1
#define OFFSET_PORT 2
#define RECV_FILENAME_ERR -32

typedef enum {INIT, RECV_FILENAME, SEND_DATA, WAIT_ACK,
              SEND_EOF, DONE} STATE;

#endif
