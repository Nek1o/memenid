#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <stdio.h>
#include <stdbool.h>

#include "../gemini_protocol/gemini_utils.h"

#define PATH_SEPARATOR "/"

typedef char *Url;

typedef char *Path;

typedef char *string;

struct Resource
{
    char *content;
    int len;
    Path path;
};

struct Response
{
    struct Resource resource;
    enum Status status;
    string meta;
};

// Resource_free cleans after Resource struct
void Resource_free(struct Resource *);

// Response_free cleans after Response struct
void Response_free(struct Response *);

// parse_request_url returns path to the requested resource
// e.g. gemini://xxx.xx/yy/zz => /yy/zz
// returns "/" if no path was presented
// !!! need cleanup !!!
Path parse_request_url(const Url);

// check_protocol returns false if protocol isn't gemini
bool check_gemini_protocol(const Url);

// get_file_content reads an entire file located at path into a buffer
// on success returns 0
// on any error returns -1
int get_file_content(struct Resource *);

// get_resource_for_serving constructs resource that should be served to the user
// returns 0 upon success
// returns 1 upon any error
int get_resource_for_serving(const Path, struct Resource*);

// construct_response takes input data from the tcp socket and constructs an apropriate response
// returns 0 upon success (any valid status)
// returns 1 upon any error
int construct_response(const char *data, const Path root_dir, struct Response *response);

int send_response(const struct Response, int socket_fd);

#endif // !SERVER_UTILS_H
