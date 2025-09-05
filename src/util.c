#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#include "util.h"
#include "bffile.h"

void strtolower(char *string)
{
    while (*string != '\0')
    {
        *string = tolower (*string);
        string++;
    }
}

void strtocapwords(char *s)
{
    TbBool word_start = 1;

    while (*s != '\0')
    {
        if (word_start) {
            if (isalpha(*s)) {
                *s = toupper(*s);
                word_start = 0;
            }
        } else {
            *s = tolower(*s);
            if (*s == ' ')
                word_start = 1;
        }
        s++;
    }
}

const char *extract_path_segment(const char *path, char *buffer, size_t size)
{
    const char *sep;
    int seg_len;

    sep = strchr (path, FS_SEP);
    if (sep == NULL)
    {
        snprintf(buffer, size, "%s", path);
        return NULL;
    }

    if (sep == path)
    {
        snprintf(buffer, size, "%s", FS_SEP_STR);
        return (path + 1);
    }

    seg_len = MIN (sep - path, (int) size);
    snprintf(buffer, size, "%.*s", seg_len, path);

    return sep;
}

