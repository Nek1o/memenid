#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "socket.h"
#include "../config.h"

// configure_tcp_socket creates a tcp socket socket according to config and cli args.
// Socket is bound and listening ready to accept connections.
int create_tcp_socket(in_addr_t host, int port)
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

    // for some reason they call it that
    int yes = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("Socket options setting failed");
        exit(EXIT_FAILURE);
    }

    if (bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("Socket binding failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(sock_fd, CONNECTION_QUEUE) == -1)
    {
        char *err_buff;
        sprintf(err_buff, "Could not listen on port %d", port);
        perror(err_buff);
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    return sock_fd;
}

// send_all sends all the data from buff via socket fd,
// len initially is a length of buff aka the number of bytes to be sent
// then len is set to the number of bytes actually sent.
int send_all(int fd, const char *buff, int *len)
{
    int total = 0; // bytes send
    int bytes_left = *len;
    int n;

    while (total < *len)
    {
        n = send(fd, buff + total, bytes_left, 0);
        if (n == -1)
        {
            perror("An error occurred during sending data via socket");
            break;
        }
        total += n;
        bytes_left -= n;
    }
    *len = total;
    return n == -1 ? -1 : 0; // 0 on success, -1 on error
}