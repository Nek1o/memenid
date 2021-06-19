#include <argp.h>
#include <arpa/inet.h>

#ifndef SERVER_ARGS_H
#define SERVER_ARGS_H

struct Arguments
{
    int port;
    in_addr_t host;
    char *root;
};

// Argument parser func
error_t parse_opt(int, char *, struct argp_state *);

// get_server_args parses cli args using parse_opt func.
// Default parameters from the config.h are set if some or all args are not present
struct Arguments get_server_args(int, char **);

#endif // !SERVER_ARGS_H
