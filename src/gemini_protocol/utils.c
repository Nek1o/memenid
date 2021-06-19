#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "utils.h"

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
static int findn(const char *str, char symbol, int n)
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

bool check_gemini_protocol(Url url)
{
    size_t semicolon_index = find(url, ':');
    if (semicolon_index == -1)
        return false;

    return strncmp(url, "gemini", semicolon_index - 1) == 0 ? true : false;
}

char *parse_request_url(Url url)
{
    int third_slash_index = findn(url, '/', 3);

    if (third_slash_index == -1)
        return "/";

    int path_size = strlen(url) - third_slash_index + 1;
    char *path = (char *)malloc(path_size);
    strncpy(path, url + third_slash_index, path_size);

    return path;
}
