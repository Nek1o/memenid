#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "socket.h"
#include "../config.h"

// configure_tcp_socket creates a tcp socket socket according to config and cli args.
// Socket is bound and listening ready to accept connections.
int configure_tcp_socket(in_addr_t host, int port)
{
    int sock_fd = -1;
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket openning failed");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in serv_addr;
    // preallocating the memory
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = host;
    serv_addr.sin_family = AF_INET;

    // TODO
    // if (setsockopt(sockfd, AF_INET, ) == -1) {
    //     perror("Socket options setting failed");
    //     exit(EXIT_FAILURE);
    // }

    if (bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sock_fd, CONNECTION_QUEUE) == -1)
    {
        char *err_buff;
        sprintf(err_buff, "Could not listen on port %d", port);
        perror(err_buff);
        exit(EXIT_FAILURE);
    }

    return sock_fd;
}