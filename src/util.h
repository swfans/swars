#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdbool.h>

#ifndef MIN
# define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
# define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifdef __unix__
# define FS_SEP '/'
# define FS_SEP_STR "/"
#elif defined _WIN32
# define FS_SEP '\\'
# define FS_SEP_STR "\\"
#endif

#ifdef _WIN32
# define strcasecmp stricmp
# define strncasecmp strnicmp
# define mkdir(path, mode) mkdir (path)
#endif


void strtolower (char *string);

const char *extract_path_segment (const char *path,
				  char *buffer, size_t size);
bool mkdir_recursive (const char *path, mode_t mode);

void *xmalloc (size_t size);
void *xcalloc (size_t size);
void xfree (void *ptr);
char *xstrdup (const char *str);

#endif
