#ifndef __CONNECTION_H
#define __CONNECTION_H

#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "lib.h"

#define BACKLOG 50
#define HOST_ERR -23

int get_socket(uint16_t port, BOOLEAN print);
int dns_lookup(char* name, struct in_addr* ip);

#endif
