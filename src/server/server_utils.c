#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "server_utils.h"
#include "../network/socket.h"
#include "../gemini_protocol/gemini_utils.h"

// -1 on not found
static int find(const char *str, char symbol)
{
    for (size_t i = 0; i < strlen(str); i++)
        if (str[i] == symbol)
            return i;

    return -1;
}

// like find, but finds the index of the nth occurrence,
// returns -1 on not finding exactly nth occurrence
static int find_n(const char *str, char symbol, int n)
{
    int counter = 0;
    for (size_t i = 0; i < strlen(str); i++)
    {
        if (str[i] == symbol)
            counter++;

        if (counter == n)
            return i;

        if (counter > n)
            break;
    }
    return -1;
}

// check_protocol returns false if protocol isn't gemini
bool check_gemini_protocol(const Url url)
{
    size_t semicolon_index = find(url, ':');
    if (semicolon_index == -1)
        return false;

    return strncmp(url, "gemini", semicolon_index - 1) == 0 ? true : false;
}

// parse_request_url returns path to the requested resource
// e.g. gemini://xxx.xx/yy/zz => /yy/zz
// returns "/" if no path was presented
// !!! need cleanup !!!
Path parse_request_url(const Url url)
{
    int third_slash_index = find_n(url, '/', 3);

    if (third_slash_index == -1)
    {
        char *path = (char *)malloc(2);
        strcpy(path, "/");
        return path;
    }

    int path_size = strlen(url) - third_slash_index + 1;
    char *path = (char *)malloc(path_size);
    strncpy(path, url + third_slash_index, path_size);

    return path;
}

void Resource_new(struct Resource *r)
{
    r->path = NULL;
    r->content = NULL;
    r->len = 0;
}

void Response_new(struct Response *r)
{
    Resource_new(&r->resource);
    strcpy(r->meta, "");
}

void Resource_free(struct Resource *r)
{
    free(r->content);
    free(r->path);
}

void Response_free(struct Response *r)
{
    Resource_free(&r->resource);
}

int get_file_content(struct Resource *resource)
{
    FILE *file = NULL;
    if ((file = fopen(resource->path, "rb")) == NULL)
    {
        perror("An error occurred during openning a file for serving");
        return 1;
    }

    fseek(file, 0, SEEK_END);  // jump to the end of the file
    int file_len = ftell(file); // get the current byte offset in the file
    rewind(file);              // jump back to the beginning of the file

    resource->content = (char *)malloc(file_len * sizeof(char)); // enough memory for the file
    if (fread(resource->content, file_len, 1, file) != 1)        // read in the entire file
    {
        perror("An error occurred during reading a file for serving");
        return 1;
    }

    fclose(file); // Close the file

    return 0;
}

int get_resource_for_serving(const Path root_dir, struct Resource *resource)
{
    // this implementation treats absent file errors and file read errors the same way
    // returning 1 upon such case

    // TODO check for dots `..` in path

    // concatenate root_dir path with requested resource path in constructed_path
    // Path constructed_path = (Path)malloc(strlen(root_dir) * sizeof(char));
    // strcpy(constructed_path, root_dir);
    // constructed_path = strcat(constructed_path, resource->path);

    // root of the capsule
    if (strcmp(resource->path, "/") == 0)
    {
        // trying index.gmi
        // constructed_path = strcat(constructed_path, "index.gmi");
        // resource->path = constructed_path;

        resource->path = (Path)realloc(resource->path, (strlen(root_dir) + 11) * sizeof(char));
        sprintf(resource->path, "%s/%s", root_dir, "index.gmi");
        if (get_file_content(resource) == 0)
            return 0;

        // trying index.gemini
        // constructed_path = strcat(constructed_path, "index.gemini");
        // resource->path = constructed_path;
        resource->path = (Path)realloc(resource->path, (strlen(root_dir) + 14) * sizeof(char));
        sprintf(resource->path, "%s/%s", root_dir, "index.gemini");
        if (get_file_content(resource) == 0)
            return 0;
    }

    Path constructed_path = (Path)malloc((strlen(root_dir) + strlen(resource->path)) * sizeof(Path));
    sprintf(constructed_path, "%s%s", root_dir, resource->path);
    // return whatever the user requested from the root directory
    resource->path = constructed_path;
    if (get_file_content(resource) != 0)
        return 1;

    return 0;
}

int construct_response(const char *data, const Path root_dir, struct Response *response)
{
    if (!check_gemini_protocol((Url)data))
    {
        // no response body, meta for additional info
        response->status = BAD_REQUEST;
        strcpy(response->meta, "Bad protocol, should be `gemini`");
        return 0;
    }

    Path path = parse_request_url((Url)data);
    response->resource.path = path;

    if (get_resource_for_serving(root_dir, &response->resource) != 0)
    {
        // no response body, meta for additional info
        response->status = NOT_FOUND;
        strcpy(response->meta, "Resource not found");
        return 0;
    }

    response->status = SUCCESS;
    // TODO deduce file MIME type from the file extension
    strcpy(response->meta, "text/gemini; lang=en\r\n");

    return 0;
}

int send_response(const struct Response response, int socket_fd)
{
    // for the send_all func
    int *len = (int *)malloc(sizeof(int));

    char header[2048];
    // send success response header
    // <STATUS><SPACE><META><CR><LF>
    if (response.status == SUCCESS)
    {
        if (sprintf(header, "%d %s\r\n", SUCCESS, response.meta) < 0)
        {
            // failed to construct a header string
            // don't know yet what to do in this case
            return -1;
        }
        *len = strlen(header);
        if ((send_all(socket_fd, header, len)) == -1)
        {
            perror("An error occurred when trying to write stuff to connection");
            return -1;
        }

        // TODO validate send_all len

        // TODO send response body

        return 0;
    }
    if (response.status / 10 == 5)
    {
        // TODO make send_header func
        if (response.status == BAD_REQUEST)
        {
            if (sprintf(header, "%d %s\r\n", BAD_REQUEST, response.meta) < 0)
            {
                // failed to construct a header string
                // don't know yet what to do in this case
                return -1;
            }
            *len = strlen(header);
            if ((send_all(socket_fd, header, len)) == -1)
            {
                perror("An error occurred when trying to write stuff to connection");
                return -1;
            }
        }
    }

    free(len);
    return 0;
}