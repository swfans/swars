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

bool
mkdir_recursive (const char *path, mode_t mode)
{
  char buffer[FILENAME_MAX];
  char *p;
  size_t len;
  struct stat st;
  int err;

  len = snprintf (buffer, sizeof (buffer), "%s", path);

  /* First, find the longest existing path */
  do
    {
      err = stat (buffer, &st);
      if (err == 0)
	{
	  if (!S_ISDIR (st.st_mode))
	    {
	      fprintf (stderr, "%s: Not a directory\n", buffer);
	      return false;
	    }
	}
      else
	{
	  if (errno != ENOENT)
	    {
	      perror (buffer);
	      return false;
	    }

	  p = strrchr (buffer, FS_SEP);
	  if (p == NULL)
	    break;

	  *p = 0;
	}
    }
  while (err != 0);

  /*
   * At this point, buffer contains the longest existing path.  Go forward
   * through the rest of the path and create the missing directories.
   */
  p = buffer;

  for (;;)
    {
      while (*p != '\0')
	p++;

      if (p >= buffer + len)
	break;

      *p = FS_SEP;

      if (mkdir (buffer, mode) != 0)
	{
	  perror (buffer);
	  return false;
	}
    }

  return true;
}
