/************************************************************************************//**
 *  @file       client.c
 *
 *  @brief      Brief descriptinon of client.c 
 *
 *  @date       2019-10-06 13:18
 *
 ***************************************************************************************/

#include "client.h"

int sockfd = 0;
bool close_session = false;

  
int main() 
{  
    struct sockaddr_in servaddr;
    pthread_t pthread;
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        perror("socket"); 
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
        perror("connection");
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  
    // create thread for send message to the server.
    if (pthread_create(&pthread, NULL, client_thread_send, NULL) != 0) {
            perror("pthread_create");
            goto end;
    }
    // create thread for send message to the server.
    if (pthread_create(&pthread, NULL, client_thread_receive, NULL) != 0) {
            perror("pthread_create");
            goto end;
    }
    // close the socket
    for(;;) {
        if(close_session) {
            printf("\n Good Bye :-) ...\n");
            break;
        }
    }

end:
    close(sockfd);

    return 0;
}

void str_trim_lf (char* arr, int length) {
  int i;
  for (i = 0; i < length; i++) { // trim \n
    if (arr[i] == '\n') {
      arr[i] = '\0';
      break;
    }
  }
}

void str_overwrite_stdout() {
  printf("%s", "> ");
  fflush(stdout);
}

void *client_thread_send(void *arg) 
{ 
    char buff[MAX_BUFF_SIZE]; 
    int n;
    char user_name[MAX_USERNAME_SIZE];

    do {
        n = 0;
        printf("Please enter your user name : ");
        fgets(user_name, MAX_USERNAME_SIZE, stdin);
        str_trim_lf(user_name, strlen(user_name));
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
        str_overwrite_stdout();
        n = 0; 
        while ((buff[n++] = getchar()) != '\n') 
            ;
        write(sockfd, buff, sizeof(buff)); 
        if ((strncmp(buff, "exit", 4)) == 0) {  
            close_session = true;
        } 
    }

    return NULL;
}

void *client_thread_receive (void *arg) {
    char buff[MAX_BUFF_SIZE];

    for(;;){
        bzero(buff, MAX_BUFF_SIZE); 
        read(sockfd, buff, sizeof(buff));
        printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET,buff);
        str_overwrite_stdout();
    }

    return NULL;
}



