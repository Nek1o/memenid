#include <openssl/ssl.h>
#include <openssl/err.h>

#ifndef OPEN_SSL_H
#define OPEN_SSL_H

void init_openssl();

void cleanup_openssl();

SSL_CTX *create_context();

void configure_context(SSL_CTX *ctx);

#endif