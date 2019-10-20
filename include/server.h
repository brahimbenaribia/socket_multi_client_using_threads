/*************************************************************************************//**
 *  @file       server.h
 *
 *  @brief      Brief description of server.h
 *
 *  @date       2019-10-06 13:25
 *         
 **************************************************************************************/ 


#ifndef SERVER_H
#define SERVER_H

#include "common.h"

#define BACKLOG            10

struct client_s {
    unsigned int client_id;
    char client_name[MAX_USERNAME_SIZE];
    int socket_fd;
    struct sockaddr_in client_address;
    struct client_s * next;
};
typedef struct client_s client_t;

void *pthread_routine(void *arg);

void signal_handler(int signal_number);

void push_client(client_t * client);

void pop(client_t * client);

void pop_client(unsigned int client_id);

client_t * get_client(int socket_fd);

void brodcast_message(char *mesg, int socket_fd);

#endif /* !SERVER_H */

