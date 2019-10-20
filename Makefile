
# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS = -Wall -g

# define any directories containing header files other than /usr/include
INCLUDES = -I./include

# define any libraries to link into executable:
LIBS = -lpthread

all: server client

server: server.o
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@ $(LIBS)

client: client.o
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@ $(LIBS)

server.o: server.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

client.o: client.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf *.o



