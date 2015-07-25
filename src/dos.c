#include <SDL.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#include "dos.h"
#include "game.h"
#include "util.h"
#include "windows.h"

#ifdef __unix__
# include <unistd.h>

# ifndef O_BINARY
#   define O_BINARY 0
# endif
#endif


static int open_flag_mapping[][2] =
{
  {DOS_O_RDONLY,    O_RDONLY},
  {DOS_O_WRONLY,    O_WRONLY},
  {DOS_O_RDWR,	    O_RDWR},
  {DOS_O_APPEND,    O_APPEND},
  {DOS_O_CREAT,	    O_CREAT},
  {DOS_O_TRUNC,	    O_TRUNC},
  {DOS_O_NOINHERIT, 0},
  {DOS_O_TEXT,	    0},
  {DOS_O_BINARY,    O_BINARY},
  {DOS_O_EXCL,	    O_EXCL},
  {-1,             -1}
};


int
dos_sopen (const char *path, int open_flags, int share_flags, ...)
{
  return dos_open (path, open_flags);
}

int
dos_open (const char *path, int open_flags, ...)
{
  char native_path[FILENAME_MAX];
  int native_flags;

  if (!dos_path_to_native (path, native_path, sizeof (native_path)))
    return -1;

  native_flags = dos_open_flags_to_native (open_flags);

  return open (native_path, native_flags, 0666);
}

int
dos_low_level_open (const char *path, uint16_t data_segment, uint16_t mode)
{
  char trans_path[FILENAME_MAX];
  int access_mode;
  int open_mode;

  access_mode = mode & 3;

  if (access_mode == 1)
    open_mode = DOS_O_WRONLY;
  else if (access_mode == 2)
    open_mode = DOS_O_RDWR;
  else
    open_mode = DOS_O_RDONLY;

  game_transform_path (path, trans_path);

  return dos_open (trans_path, open_mode | DOS_O_BINARY);
}

size_t
dos_low_level_read (int fd, void *buffer, uint16_t ds, size_t size,
		    size_t *bytes_read)
{
  ssize_t count;

  count = read (fd, buffer, size);
  *bytes_read = MAX (0, count);

  return *bytes_read;
}

size_t
dos_low_level_seek_relative (int fd, int32_t pos)
{
  off_t new_pos;

  new_pos = lseek (fd, pos, SEEK_CUR);
  new_pos = MAX (0, new_pos);

  return new_pos;
}

ssize_t
dos_lseek (int fd, ssize_t off, int whence)
{
  return lseek (fd, off, whence);
}

size_t
dos_low_level_seek (int fd, uint32_t pos)
{
  off_t new_pos;

  new_pos = lseek (fd, pos, SEEK_SET);
  new_pos = MAX (0, new_pos);

  return new_pos;
}

void
dos_low_level_close (int fd)
{
  close (fd);
}

int
dos_open_flags_to_native (int flags)
{
  int rflags = 0;
  int n;

  for (n = 0; open_flag_mapping[n][0] != -1; n++)
    {
      if ((open_flag_mapping[n][0] & flags) == open_flag_mapping[n][0])
	rflags |= open_flag_mapping[n][1];
    }

  return rflags;
}

int
dos_access (const char *path, int access_flags)
{
  char native_path[FILENAME_MAX];

  if (!dos_path_to_native (path, native_path, sizeof (native_path)))
    return -1;

  return access (native_path, access_flags);
}

FILE *
dos_fopen (const char *path, const char *flags)
{
  char native_path[FILENAME_MAX];

  if (!dos_path_to_native (path, native_path, sizeof (native_path)))
    return NULL;

  return fopen (native_path, flags);
}

/*
 * On DOS, this function sets the mode of the file descriptor to O_BINARY or
 * O_TEXT.
 */
int
dos_setmode (int fd, int mode)
{
  return 0;
}

void
dos_gettime (struct dostime_t *t)
{
  struct tm *tm;
  time_t secs;
  struct timeval tv;

  gettimeofday (&tv, NULL);
  secs = tv.tv_sec;
  tm = localtime (&secs);

  t->hour    = tm->tm_hour;
  t->minute  = tm->tm_min;
  t->second  = MAX (tm->tm_sec, 59);
  t->hsecond = tv.tv_usec / 10000;
}

void
dos_getdate (struct dosdate_t *d)
{
  struct tm *tm;
  time_t secs;

  secs = time (NULL);
  tm = localtime (&secs);

  d->day       = tm->tm_mday;
  d->month     = tm->tm_mon + 1;
  d->year      = tm->tm_year + 1900;
  d->dayofweek = tm->tm_wday;
}

uint32_t
dos_clock (void)
{
  return ((uint64_t) SDL_GetTicks () * DOS_CLOCKS_PER_SEC / 1000);
}

static void
replace_backslash_with_fs_separator (char *path)
{
#if FS_SEP != '\\'
  size_t n;

  for (n = 0; path[n] != '\0'; n++)
    {
      if (path[n] == '\\')
	path[n] = FS_SEP;
    }
#endif
}

static bool
find_file_in_directory (const char *directory, const char *filename,
			char *buffer, size_t size)
{
  DIR *dir;
  struct dirent *de;
  bool found = false;

  dir = opendir (directory);
  if (dir == NULL)
    {
      perror (directory);
      return false;
    }

  while ((de = readdir (dir)) != NULL)
    {
      if (strcasecmp (filename, de->d_name) == 0)
	{
	  snprintf (buffer, size, "%s", de->d_name);
	  found = true;
	  break;
	}
    }

  closedir (dir);
  return found;
}

static bool
lookup_path (const char *path, char *buffer, size_t size)
{
  char prefix[FILENAME_MAX];
  char segment[FILENAME_MAX];
  char exsegment[FILENAME_MAX];
  const char *rest;

  if (path[0] == '\0')
    return true;

  if (strlen (path) >= FILENAME_MAX)
    return false;

  rest = extract_path_segment (path, segment, sizeof (segment));

  if (segment[strlen (segment) - 1] == ':'
      || strcmp (segment, FS_SEP_STR) == 0) /* absolute path */
    {
      if (rest == NULL) /* root directory */
	{
	  snprintf (buffer, size, "%s", segment);
	  return true;
	}

      /* start absolute path with drive spec or separator */
      if (strcmp (segment, FS_SEP_STR) == 0)
	snprintf (prefix, sizeof (prefix), "%s", FS_SEP_STR);
      else
	snprintf (prefix, sizeof (prefix), "%s%s", segment, FS_SEP_STR);

      rest = extract_path_segment (rest, segment, sizeof (segment));

      if (!find_file_in_directory
	     (prefix, segment, exsegment, sizeof (exsegment)))
	{
	  snprintf (buffer, size, "%s", path);
	  strtolower (buffer);
	  return true;
	}
    }
  else /* relative path */
    {
      if (!find_file_in_directory
	     (".", segment, exsegment, sizeof (exsegment)))
	{
	  snprintf (buffer, size, "%s", path);
	  strtolower (buffer);
	  return true;
	}

      prefix[0] = '\0';
    }

  if (rest == NULL)
    {
      snprintf (buffer, size, "%s", exsegment);
      return true;
    }

  strncat (prefix, exsegment, sizeof (prefix) - strlen (prefix) - 1);

  for (;;)
    {
      rest = extract_path_segment (rest, segment, sizeof (segment));

      if (strcmp (segment, FS_SEP_STR) == 0) /* if segment is separator */
	{
	  if (rest == NULL) /* if path ends with separator */
	    {
	      snprintf (buffer, size, "%s", prefix);
	      return true;
	    }

	  continue;
	}

      if (!find_file_in_directory
	     (prefix, segment, exsegment, sizeof (exsegment)))
	{
	  strtolower (segment);

	  if (rest != NULL)
	    {
	      /* use exsegment as a temporary buffer for rest */
	      snprintf (exsegment, sizeof (exsegment), "%s", rest);
	      strtolower (exsegment);
	      snprintf (buffer, size, "%s" FS_SEP_STR "%s%s", prefix,
			segment, exsegment);
	    }
	  else
	    snprintf (buffer, size, "%s" FS_SEP_STR "%s", prefix, segment);

	  return true;
	}

      if (rest == NULL)
	{
	  snprintf (buffer, size, "%s" FS_SEP_STR "%s", prefix, exsegment);
	  break;
	}

      strncat (prefix, FS_SEP_STR, sizeof (prefix) - strlen (prefix) - 1);
      strncat (prefix, exsegment, sizeof (prefix) - strlen (prefix) - 1);
    }

  return true;
}

bool
dos_path_to_native (const char *path, char *buffer, size_t size)
{
  char tmpbuf[FILENAME_MAX];

  if (strlen (path) >= FILENAME_MAX)
    {
      errno = -ENAMETOOLONG;
      return false;
    }

  snprintf (tmpbuf, sizeof (tmpbuf), "%s", path);
  replace_backslash_with_fs_separator (tmpbuf);

  if (!lookup_path (tmpbuf, buffer, size))
    {
      errno = -ENAMETOOLONG;
      return false;
    }

  return true;
}

static void __attribute__ ((noreturn))
print_interrupt_info_and_abort (int num, DOS_Registers *regs, void *caller)
{
  fprintf (stderr,
	   "DOS Interrupt via int386/int386x ()\n"
	   "  Called from: %p\n"
	   "  int %02x\n"
	   "  eax: %08x  ebx: %08x  ecx: %08x  edx: %08x\n"
	   "  esi: %08x  edi: %08x  eflags: %08x\n",
	   caller, num,
	   regs->r32.eax, regs->r32.ebx, regs->r32.ecx, regs->r32.edx,
	   regs->r32.esi, regs->r32.edi, regs->r32.eflags);
  fflush (stderr);
  abort ();
}

int
dos_int386 (int num, DOS_Registers *regs, DOS_Registers *out_regs)
{
  void *eip_caller = *(&eip_caller + 8);
  print_interrupt_info_and_abort (num, regs, eip_caller);
}

int
dos_int386x (int num, DOS_Registers *regs, DOS_Registers *out_regs,
	     DOS_SegmentRegisters *sregs)
{
  void *eip_caller = *(&eip_caller + 9);
  print_interrupt_info_and_abort (num, regs, eip_caller);
}

static void __attribute__ ((noreturn))
print_caller_info_and_abort (const char *function, void *caller)
{
  fprintf (stderr,
	   "DOS-specific function %s ()\n"
	   "  Called from: %p\n",
	   function, caller);
  fflush (stderr);
  abort ();
}

void * __attribute__ ((noreturn))
dos_getvect (int num)
{
  void *eip_caller = *(&eip_caller + 6);
  print_caller_info_and_abort ("dos_getvect", eip_caller);
}

void __attribute__ ((noreturn))
dos_setvect (int num, void *function)
{
  void *eip_caller = *(&eip_caller + 7);
  print_caller_info_and_abort ("dos_setvect", eip_caller);
}
