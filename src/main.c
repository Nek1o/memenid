#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "config.h"
#include "args/server_args.h"
#include "network/socket.h"
#include "network/open_ssl.h"
#include "signals/signals.h"
#include "gemini_protocol/gemini_utils.h"
#include "server/server_utils.h"

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

    struct Arguments args = get_server_args(argc, argv);

    SSL_CTX *ctx;

    int conn_fd = -1;

    init_openssl();
    ctx = create_context();

    configure_context(ctx);

    int sock_fd = create_tcp_socket(args.host, args.port);

    while (keep_running)
    {

        char conn_buff[MAX_URL_SIZE];
        if ((conn_fd = accept(sock_fd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            perror("An error occurred during openning a new connection");
            continue;
        }

        SSL *ssl;
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, conn_fd);
        if (SSL_accept(ssl) <= 0)
        {
            ERR_print_errors_fp(stderr);
            memset(conn_buff, 0, MAX_URL_SIZE);

            SSL_shutdown(ssl);
            SSL_free(ssl);

            close(conn_fd);
            continue;
        }
        if ((SSL_read(ssl, conn_buff, MAX_URL_SIZE)) == -1)
        {
            perror("An error occurred during reading from socket");
            close(conn_fd);
            continue;
        }

        printf("Successfully received a request %s\n", conn_buff);

        struct Response response;
        Response_new(&response);
        construct_response(conn_buff, args.root, &response);

        send_response(ssl, response, conn_fd);

        Response_free(&response);
        memset(conn_buff, 0, MAX_URL_SIZE);

        SSL_shutdown(ssl);
        SSL_free(ssl);

        close(conn_fd);
    }

    close(sock_fd);
    SSL_CTX_free(ctx);
    cleanup_openssl();
    exit(EXIT_SUCCESS);
}
