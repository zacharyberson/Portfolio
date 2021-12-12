CC= gcc
CFLAGS= -g

LIBS =

all:  myClient myServer

myClient: tcp_client.c networks.o testing.o
	$(CC) $(CFLAGS) -o myClient -Wall tcp_client.c testing.o networks.o $(LIBS)

myServer: tcp_server.c networks.o testing.o
	$(CC) $(CFLAGS) -o myServer -Wall tcp_server.c testing.o networks.o $(LIBS)

networks.o: networks.c networks.h
	$(CC) $(CFLAGS) -c networks.c
	
testing.o: testing.c testing.h
	   $(CC) $(CFLAGS) -c testing.c
clean:
	rm -f myServer myClient *.o



