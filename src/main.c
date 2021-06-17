#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "config/server_args.h"

int main(int argc, char **argv)
{
    struct Arguments args = get_server_args(argc, argv);

    int sock_fd = -1;
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket openning failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in serv_addr;
    // preallocating the memory
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_port = htons(args.port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;

    // if (setsockopt(sockfd, AF_INET, ) == -1) {
    //     perror("Socket options setting failed");
    //     exit(EXIT_FAILURE);
    // }

    if (bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sock_fd, 50) == -1)
    {
        char *err_buff;
        sprintf(err_buff, "Could not listen on port %d", args.port);
        perror(err_buff);
        exit(EXIT_FAILURE);
    }

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