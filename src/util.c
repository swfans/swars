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

void
strtolower (char *string)
{
  while (*string != '\0')
    {
      *string = tolower (*string);
      string++;
    }
}

void *
xmalloc (size_t size)
{
  void *ret;

  ret = malloc (size);
  assert (ret != NULL);

  return ret;
}

void *
xcalloc (size_t size)
{
  void *ret;

  ret = calloc (1, size);
  assert (ret != NULL);

  return ret;
}

void
xfree (void *ptr)
{
  if (ptr == NULL)
    return;

  free (ptr);
}

char *
xstrdup (const char *str)
{
  char *ret;
  size_t size;

  size = strlen (str) + 1;

  ret = xmalloc (size);
  memcpy (ret, str, size);

  return ret;
}

const char *
extract_path_segment (const char *path, char *buffer, size_t size)
{
  const char *sep;
  int seg_len;

  sep = strchr (path, FS_SEP);
  if (sep == NULL)
    {
      snprintf (buffer, size, "%s", path);
      return NULL;
    }

  if (sep == path)
    {
      snprintf (buffer, size, "%s", FS_SEP_STR);
      return (path + 1);
    }

  seg_len = MIN (sep - path, (int) size);
  snprintf (buffer, size, "%.*s", seg_len, path);

  return sep;
}

