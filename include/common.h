/*************************************************************************************//**
 *  @file       common.h
 *
 *  @brief      Brief description of common.h
 *
 *  @date       2019-10-06 13:25
 *         
 **************************************************************************************/ 

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include<arpa/inet.h>
#include <stdbool.h>

#define PORT               8181
#define MAX_BUFF_SIZE      255
#define MAX_USERNAME_SIZE  10

void str_trim_lf (char* arr, int length);
void str_overwrite_stdout();

#endif /* !COMMON_H */