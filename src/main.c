#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "config.h"
#include "args/server_args.h"
#include "network/socket.h"
#include "signals/signals.h"

// main while loop condition variable
static volatile sig_atomic_t keep_running = 1;

// sigint_handler inverts the main while loop condition variable
// and therefore stops the program
// void sigint_handler(int signum)
// {
//     write(1, "Graceful shutdown", 17);
//     keep_running = 0;
// }

int main(int argc, char **argv)
{
    // signal(SIGINT, sigint_handler);
    // TODO signals handling for proper socket closing
    // fuser -k <port>/tcp

    struct Arguments args = get_server_args(argc, argv);

    int sock_fd = configure_tcp_socket(args.host, args.port);

    int conn_fd = -1;
    while (keep_running)
    {
        char conn_buff[MAX_URL_SIZE];
        if ((conn_fd = accept(sock_fd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            perror("An error occurred during openning a new connection");
            continue;
        }

        int read_res = read(conn_fd, conn_buff, MAX_URL_SIZE);

        printf("Successfully received a request %s", conn_buff);

        switch (read_res)
        {
        case -1:
            perror("An error occurred during reading from socket");
            close(conn_fd);
            break;
        case 0:
            conn_buff[read_res] = '\0';
            printf("Read returned 0");
            break;
        }
        
        int *conn_len;
        *conn_len = strlen(conn_buff);
        if ((send_all(conn_fd, conn_buff, conn_len)) == -1)
        {
            perror("An error occurred when trying to write stuff to connection");
            continue;
        }

        // printf("DATA: %s", conn_buff);
        memset(conn_buff, 0, MAX_URL_SIZE);
        close(conn_fd);
        sleep(1);
    }

    close(sock_fd);
    exit(EXIT_SUCCESS);
}