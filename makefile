CC = g++
CFLAGS = -std=c++11
PFLAGS = -pthread
 
all: clientmain.cpp servermain.cpp server.o client.o common.o
	$(CC) $(CFLAGS) servermain.cpp  server.o common.o -o server 
	$(CC) $(CFLAGS) $(PFLAGS) clientmain.cpp client.o common.o -o client

common.o: common.cpp common.h
	$(CC) $(CFLAGS) -c common.cpp

server.o: server.cpp server.h common.h
	$(CC) $(CFLAGS) -c server.cpp
 
client.o: client.cpp client.h common.h
	$(CC) $(CFLAGS) $(PFLAGS) -c client.cpp


 
clean:
	rm -f *.o server client common