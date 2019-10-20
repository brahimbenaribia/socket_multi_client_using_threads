/************************************************************************************//**
 *  @file       server.c
 *
 *  @brief      Brief descriptinon of server.c 
 *
 *  @date       2019-10-06 13:18
 *
 ***************************************************************************************/

#include "server.h"
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
client_t *head = NULL;
static unsigned int client_inc = 0;
static unsigned int client_nb = 0;


int main()
{
    int socket_fd, new_socket_fd;
    struct sockaddr_in address;
    //pthread_attr_t pthread_attr;
    client_t *client_arg;
    pthread_t pthread;
    socklen_t client_address_len;

    /* Initialise IPv4 address. */
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    /* Create TCP socket. */
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* Bind address to socket. */
    if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("bind");
        exit(1);
    }

    /* Listen on socket. */
    if (listen(socket_fd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    /* Assign signal handlers to signals. */
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(1);
    }
    if (signal(SIGTERM, signal_handler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    while (1) {
        client_arg = (client_t *)calloc(1, sizeof(client_t));
        if(!client_arg) {
            perror("malloc");
            continue;
        }

        client_address_len = sizeof(client_arg->client_address);
        new_socket_fd = accept(socket_fd, (struct sockaddr *)&client_arg->client_address, &client_address_len);
        if (new_socket_fd == -1) {
            perror("accept");
            free(client_arg);
            continue;
        }

        /* Initialise pthread argument. */
        client_arg->socket_fd = new_socket_fd;
        push_client(client_arg);
        if (pthread_create(&pthread, NULL, pthread_routine, (void *)&client_arg->socket_fd) != 0) {
            perror("pthread_create");
            free(client_arg);
            continue;
        }
    }

    return 0;
}

void push_client(client_t * client) {
    client_t *current = head;

    client->client_id = client_inc;
    client->next = NULL;
    client_inc ++;
    client_nb ++;
    if (head == NULL) {
        head = client;
        return;
    }
    while (current->next != NULL)
        current = current->next;
    current->next = client;

    return;
}

void pop(client_t * client) {
    client_t * tmp_client = NULL;

    if (client == head) {
        head = client->next;
        free(client);
        return;
    }
    tmp_client = client->next;
    client->next = tmp_client->next;
    free(tmp_client);

    return;
}

void pop_client(unsigned int client_id) {
    client_t *current = head;
    client_t *previous = head;
    int i = 0;

    for (i = 0; i <= client_nb; i++) {
        if (current->client_id == client_id) {
            pop(previous);
            client_nb --;
            return;
        }
        previous = current;
        current = current->next;
    }

    return;
}

client_t * get_client(int socket_fd) {
    client_t *current = head;

    while (current != NULL) {
        if(current->socket_fd == socket_fd) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

__attribute__((unused))
void brodcast_message(char *mesg, int socket_fd) {
    client_t *current = head;

    while (current != NULL) {
        if(current->socket_fd != socket_fd)
            write(current->socket_fd, mesg, sizeof(mesg));
        current = current->next;
    }

    return;
}

void *pthread_routine(void *arg)
{
    client_t *client_arg;
    int socket_fd = *(int *) arg;
    char buff[MAX_BUFF_SIZE];
    bool login = true;
    //char entete[MAX_BUFF_SIZE];
    
    for (;;) {
        bzero(buff, MAX_BUFF_SIZE);
        if (login) {
            client_arg = get_client(socket_fd);
            read(socket_fd, buff, sizeof(buff));
            strcpy(client_arg->client_name, buff);
            /* sprintf(entete,"************ Message from : %s ************\n",
            client_arg->client_name); */
            login = false; 
            continue;
        }
        // read the message from client and copy it in buffer
        read(socket_fd, buff, sizeof(buff));
        // print buffer which contains the client contents 
        printf("From client %s : %s\n", client_arg->client_name, buff);
        /* brodcast_message(entete,socket_fd);
        brodcast_message(buff,socket_fd);
        brodcast_message("EOM",socket_fd); */
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Client %d  Exit...\n", client_arg->client_id);
            pop_client(client_arg->client_id);
            break; 
        }
        bzero(buff, MAX_BUFF_SIZE); 
        // copy server message in the buffer 
        /*while ((buff[n++] = getchar()) != '\n') 
            ; */
        // and send that buffer to client 
        //write(socket_fd, buff, sizeof(buff));
    }

    return NULL;
}

void signal_handler(int signal_number) {
    /* TODO: Put exit cleanup code here. */
    exit(0);
}





