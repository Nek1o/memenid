#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "config.h"
#include "args/server_args.h"
#include "network/socket.h"

int main(int argc, char **argv)
{
    struct Arguments args = get_server_args(argc, argv);

    int sock_fd = configure_tcp_socket(args.host, args.port);

    int conn_fd = -1;
    while (1)
    {
        char conn_buff[128] = "Hello, World!";
        if ((conn_fd = accept(sock_fd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            perror("An error ocured during opennin a new connection");
            continue;
        }
        if ((write(conn_fd, conn_buff, strlen(conn_buff))) == -1)
        {
            perror("An error ocured when trying to write stuff to connection");
            continue;
        }

        // printf("DATA: %s", conn_buff);

        close(conn_fd);
        sleep(1);
    }

    close(sock_fd);
    exit(EXIT_SUCCESS);
}