/************************************************************************************//**
 *  @file       client.c
 *
 *  @brief      Brief descriptinon of client.c 
 *
 *  @date       2019-10-06 13:18
 *
 ***************************************************************************************/

#include "server.h"

void client_thread(int sockfd) 
{ 
    char buff[MAX_BUFF_SIZE]; 
    int n;
    char user_name[MAX_USERNAME_SIZE];
    do {
        n = 0;
        printf("Please enter your user name : ");
        scanf("%s",user_name);
        /* get Enter key '\n' */
        getchar();
        if (strlen(user_name) < 3) {
            printf("User name is too short - must be at least 3 characters\n");
            printf("Please re-entre again\n");
        } else {
            write(sockfd, user_name, sizeof(user_name));
            break;
        }
    }while(1);
    
    for (;;) { 
        bzero(buff, sizeof(buff)); 
        printf("Enter your message : "); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n') 
            ;
        write(sockfd, buff, sizeof(buff)); 
        //bzero(buff, sizeof(buff)); 
        //read(sockfd, buff, sizeof(buff)); 
        //printf("From Server : %s", buff); 
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break; 
        } 
    } 
} 
  
int main() 
{ 
    int sockfd; 
    struct sockaddr_in servaddr; 
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    // connect the client socket to server socket 
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  
    // function for chat 
    client_thread(sockfd); 
  
    // close the socket 
    close(sockfd); 
} 



