
#ifndef MIME_H
#define MIME_H

#include <stdlib.h>
#include <string.h>

#include "server_utils.h"

#define kar char *

struct MIMEType
{
    char type[256];
    char ext[128];
};

static struct MIMEType MIMETypes[] = {
    {"text/gemini", "gmi"},
    {"text/gemini", "gemini"},
    {"audio/aac", "aac"},
    {"application/x-abiword", "abw"},
    {"application/x-abiword", "abw"},
    {"application/x-freearc", "arc"},
    {"video/x-msvideo", "avi"},
    {"application/vnd.amazon.ebook", "azw"},
    {"application/octet-stream", "bin"},
    {"image/bmp", "bmp"},
    {"application/x-bzip", "bz"},
    {"application/x-bzip2", "bz2"},
    {"application/x-cdf", "cda"},
    {"application/x-csh", "csh"},
    {"text/css", "css"},
    {"text/csv", "csv"},
    {"application/msword", "doc"},
    {"application/vnd.openxmlformats-officedocument.wordprocessingml.document", "docx"},
    {"application/vnd.ms-fontobject", "eot"},
    {"application/epub+zip", "epub"},
    {"application/gzip", "gz"},
    {"image/gif", "gif"},
    {"text/html", "html"},
    {"text/html", "htm"},
    {"image/vnd.microsoft.icon", "ico"},
    {"text/calendar", "ics"},
    {"application/java-archive", "jar"},
    {"image/jpeg", "jpeg"},
    {"image/jpeg", "jpg"},
    {"text/javascript", "js"},
    {"application/json", "json"},
    {"application/ld+json", "jsonld"},
    {"audio/midi", "mid"},
    {"audio/midi", "midi"},
    {"text/javascript", "mjs"},
    {"audio/mpeg", "mp3"},
    {"video/mp4", "mp4"},
    {"video/mpeg", "mpeg"},
    {"application/vnd.apple.installer+xml", "mpkg"},
    {"application/vnd.oasis.opendocument.presentation", "odp"},
    {"application/vnd.oasis.opendocument.spreadsheet", "ods"},
    {"application/vnd.oasis.opendocument.text", "odt"},
    {"audio/ogg", "oga"},
    {"video/ogg", "ogv"},
    {"image/png", "png"},
    {"application/pdf", "pdf"},
    {"application/x-httpd-php", "php"},
    {"application/vnd.ms-powerpoint", "ppt"},
    {"application/vnd.openxmlformats-officedocument.presentationml.presentation", "pptx"},
    {"application/vnd.rar", "rar"},
    {"application/rtf", "rtf"},
    {"application/x-sh", "sh"},
    {"image/svg+xml", "svg"},
    {"application/x-shockwave-flash", "swf"},
    {"application/x-tar", "tar"},
    {"image/tiff", "tif"},
    {"video/mp2t", "tiff"},
    {"video/mp2t", "ts"},
    {"font/ttf", "ttf"},
    {"text/plain", "txt"},
    {"application/vnd.visio", "vsd"},
    {"audio/wav", "wav"},
    {"audio/webm", "weba"},
    {"video/webm", "webm"},
    {"image/webp", "webp"},
    {"font/woff", "woff"},
    {"font/woff2", "woff2"},
    {"application/xhtml+xml", "xhtml"},
    {"application/vnd.ms-excel", "xls"},
    {"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet", "xlsx"},
    {"application/xml", "xml"},
    {"text/xml", "xml"},
    {"application/vnd.mozilla.xul+xml", "xul"},
    {"application/zip", "zip"},
    {"audio/3gpp", "3gp"},
    {"video/3gpp", "3gp"},
    {"audio/3gpp2", "3g2"},
    {"video/3gpp2", "3g2"},
    {"application/x-7z-compressed", "7z"},
    {"", ""}};

// -1 on error
// bin if no file extension was specified
// if file has multiple extensions, ext will be set to binary anyways.
int get_file_ext(const Path path, kar ext);

// -1 on error
int deduce_mime_type(const Path path, MetaString meta);

#endif // !MIME_H
