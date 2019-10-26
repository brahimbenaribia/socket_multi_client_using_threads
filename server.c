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

void pop_client(int socket_fd) {
    client_t *current = head;
    client_t *previous = head;

    while (current != NULL) {
        if (current->socket_fd == socket_fd) {
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

void brodcast_message(char *mesg, int socket_fd) {
    client_t *current = head;

    while (current != NULL) {
        if(current->socket_fd != socket_fd)
            write(current->socket_fd, mesg, MAX_BUFF_SIZE);
        current = current->next;
    }

    return;
}

void *pthread_routine(void *arg)
{
    client_t *client_arg;
    int socket_fd = *(int *) arg;
    char buff[MAX_BUFF_SIZE];
    char client_name[MAX_USERNAME_SIZE];
    char broadcast_mesg[MAX_BUFF_SIZE + 32];

    read(socket_fd, buff, sizeof(buff));
    strcpy(client_name, buff);
    client_arg = get_client(socket_fd);
    strcpy(client_arg->client_name, client_name);
    sprintf(broadcast_mesg, "%s join the connection ...\n", client_arg->client_name);
    brodcast_message(broadcast_mesg, socket_fd);
    for (;;) {
        bzero(buff, MAX_BUFF_SIZE);
        // read the message from client and copy it in buffer
        read(socket_fd, buff, sizeof(buff));
        if (strlen(buff) > 1) {
            printf("Mesg from client %s : %s", client_name, buff);
            sprintf(broadcast_mesg, "%s : %s", client_name, buff);
            brodcast_message(broadcast_mesg, socket_fd);
        }
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) {
            pop_client(socket_fd);
            printf("Client %s  Exit...\n", client_name);
            break; 
        }
    }

    return NULL;
}

void signal_handler(int signal_number) {
    /* TODO: Put exit cleanup code here. */
    exit(0);
}





