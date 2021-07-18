#include "mime.h"

int get_file_ext(const Path path, kar ext)
{
    int path_len = strlen(path);
    if (path_len <= 0)
        return -1;

    kar found_ext = strrchr((const char *)path, '.');
    if (found_ext == NULL)
    {
        strcpy(ext, ".bin");
        return 0;
    }

    int found_ext_len = strlen(found_ext);

    if (found_ext_len <= 0 || found_ext_len == path_len || found_ext_len > 128)
        return -1;

    strcpy(ext, found_ext + 1);
    return 0;
}

// -1 on error
int deduce_mime_type(const Path path, MetaString meta)
{
    char ext[128];
    if (get_file_ext(path, ext) == -1)
    {
        strcpy(meta, "application/octet-stream");
        return -1;
    }

    size_t i = 0;
    while (strlen(MIMETypes[i].ext) > 0)
    {
        if (strcmp(MIMETypes[i].ext, ext) == 0)
        {
            strcpy(meta, MIMETypes[i].type);
            return 0;
        }
        ++i;
    }

    strcpy(meta, "application/octet-stream");
    return 0;
}