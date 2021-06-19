#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

typedef char *Url;

// parse_request_url returns path to the requested resource
// e.g. gemini://xxx.xx/yy/zz => /yy/zz
// returns "/" if no path was presented 
// !!! need cleanup !!!
char *parse_request_url(Url);

// check_protocol returns false if protocol isn't gemini
bool check_gemini_protocol(Url);

#endif // !UTILS_H