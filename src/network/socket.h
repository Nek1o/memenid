#include <arpa/inet.h>

#ifndef SOCKET_H
#define SOCKET_H

// configure_tcp_socket creates a tcp socket socket according to config and cli args.
// Socket is bound and listening ready to accept connections.
int create_tcp_socket(in_addr_t, int);

// send_all sends all the data from buff via socket fd
// len is set to the number of bytes sent
int send_all(int, const char *, int *);

#endif // !SOCKET_H