#ifdef __unix__

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "unix.h"
#include "util.h"

void
unix_restore_signal_handlers (void)
{
  signal (SIGINT, SIG_DFL);
  signal (SIGTERM, SIG_DFL);
}

bool
sys_get_user_path (char *buffer, size_t size)
{
  char *xdg_config_home;
  char *home;

  xdg_config_home = getenv ("XDG_CONFIG_HOME");
  if (xdg_config_home != NULL)
    {
      snprintf (buffer, size,
		"%s" FS_SEP_STR "swars", xdg_config_home);
      return true;
    }

  home = getenv ("HOME");
  if (home == NULL)
    {
      fprintf (stderr, "Warning: HOME is not set.\n");
      return false;
    }

#ifndef __APPLE__
  snprintf (buffer, size,
            "%s" FS_SEP_STR ".config" FS_SEP_STR "swars", home);
#else
  snprintf (buffer, size,
            "%s/Library/Preferences/Syndicate Wars", home);
#endif

  return true;
}

bool
sys_get_data_path (char *buffer, size_t size)
{
  snprintf (buffer, size, GAME_DATA_PATH);
  return true;
}

#endif
