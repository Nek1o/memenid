#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <arpa/inet.h>

#include "server_args.h"
#include "config.h"

static char doc[] = DOC;
static char args_doc[] = ARGS_DOC;

// Argument parser func
error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct Arguments *arguments = state->input;
    switch (key)
    {
    case 'p':
        // if atoi failed, show usage
        if ((arguments->port = atoi(arg)) == 0)
            argp_usage(state);
        break;
    case 'h':
        if (inet_addr(arg) == -1) {
            argp_usage(state);
        }
        arguments->host = arg;
        break;
    case ARGP_KEY_ARG:
        return 0;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

// get_server_args parses cli args using parse_opt func.
// Default parameters from the config.h are set if some or all args are not present
struct Arguments get_server_args(int argc, char **argv)
{
    // Usage of OPTION_ARG_OPTIONAL flag requires all arguments to be pointers
    // I debugged this for too long...
    struct argp_option options[] = {
        {"port", 'p', "PORT", 0, "Server port"},
        {"host", 'h', "HOST", 0, "Server host"},
        {0}};

    struct argp argp = {options, parse_opt, args_doc, doc};

    // setting default arguments
    struct Arguments args = {DEFAULT_PORT, DEFAULT_HOST};

    // getting arguments if any and all the argp magic
    argp_parse(&argp, argc, argv, 0, 0, &args);

    return args;
}
