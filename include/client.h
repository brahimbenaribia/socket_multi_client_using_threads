/*************************************************************************************//**
 *  @file       client.h
 *
 *  @brief      Brief description of client.h
 *
 *  @date       2019-10-06 13:25
 *         
 **************************************************************************************/ 


#ifndef CLIENT_H
#define CLIENT_H

#include "common.h"

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void *client_thread_send(void *);
void *client_thread_receive(void *);

#endif /* !CLIENT_H */