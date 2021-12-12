#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
//#include <sys/stat.h>
//#include <sys/uio.h>
#include <sys/time.h>
//#include <unistd.h>
//#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "cpe464.h"
#include "connection.h"
#include "lib.h"

int get_socket(uint16_t port, BOOLEAN print) {
   int sock = 0;
   int status = 0;
   struct sockaddr_in addr;
   socklen_t len = sizeof(addr);

   sock = socket(AF_INET, SOCK_DGRAM, 0);
   if(sock < 0) {
      perror("server socket call");
      exit(-2);
   }
   
   addr.sin_family = AF_INET;
   addr.sin_port = htons(port);
   addr.sin_addr.s_addr = INADDR_ANY;

   status = bindMod(sock, (struct sockaddr *) &addr, len);
   if(status < 0) {
      perror("bind call");
      exit(-3);
   }
   
   if(-1 == getsockname(sock, (struct sockaddr*)&addr, &len))
      perror("getsockname");
   if(print)
      printf("Server is using port %d\n", ntohs(addr.sin_port));
   
   return sock;   
}

int dns_lookup(char* name, struct in_addr* ip) {
   struct hostent* host = NULL;
   
   host = gethostbyname(name);
   memcpy(ip,host->h_addr,4);
   
   if(!host) {
      perror("gethostbyname");
      exit(HOST_ERR);
   }
   
   return 0;
}
