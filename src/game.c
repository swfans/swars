#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "bflib_fileio.h"
#include "bflib_video.h"
#include "game_data.h"
#include "display.h"
#include "dos.h"
#include "game.h"
#include "keyboard.h"
#include "mouse.h"
#include "sound.h"
#include "unix.h"
#include "util.h"
#include "windows.h"

#include "timer.h"

#define SAVEGAME_PATH "qdata/savegame/"

extern uint8_t game_music_track;

bool
game_initialise (void)
{
  if (SDL_Init (SDL_INIT_JOYSTICK | SDL_INIT_VIDEO
		| SDL_INIT_NOPARACHUTE) != 0)
    {
      fprintf (stderr, "SDL_Init(): %s\n", SDL_GetError ());
      return false;
    }

#ifdef __unix__
  /* clean up after SDL messing around where it shouldn't */
  unix_restore_signal_handlers ();
#endif

  keyboard_initialise ();
  mouse_initialise ();
  display_initialise ();
  sound_initialise ();

  // This fills the path variable; for user, it also creates the folder
  GetDirectoryHdd();
  GetDirectoryUser();

  if ( cmdln_param_w == 1 )
  {
      buffer_allocs[35].field_A = 1;
      buffer_allocs[28].field_A = 1;
      buffer_allocs[36].field_A = 1;
      buffer_allocs[27].field_A = 1000;
      buffer_allocs[26].field_A = 1124;
      buffer_allocs[31].field_A = 2500;
      buffer_allocs[32].field_A = 1000;
      buffer_allocs[33].field_A = 700;
      buffer_allocs[30].field_A = 3000;
      if ( cmdln_param_mp || cmdln_param_bcg )
      {
        buffer_allocs[20].field_A = 2000;
        buffer_allocs[21].field_A = 2000;
        buffer_allocs[22].field_A = 2000;
      }
      buffer_allocs[4].field_A = 11000;
      buffer_allocs[9].field_A = 11000;
      buffer_allocs[5].field_A = 1500;
      buffer_allocs[7].field_A = 1000;
      buffer_allocs[13].field_A = 16000;
      buffer_allocs[14].field_A = 9000;
      mem_alloc_size = 2700000;
      game_perspective = (buffer_allocs[5].field_A >> 8) & 0xff;
  }
  if ( !cmdln_param_mp )
    cmdln_param_bcg = 1;

  return true;
}

void
game_handle_sdl_events (void)
{
  const size_t max_events = 256;
  size_t event;
  SDL_Event ev;

  for (event = 0; event < max_events; event++)
    {
      if (SDL_PollEvent (&ev) == 0)
	break;

      switch (ev.type)
	{
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONUP:
	case SDL_MOUSEBUTTONDOWN:
	  mouse_handle_event (&ev);
	  break;

	case SDL_KEYUP:
	case SDL_KEYDOWN:
	  keyboard_handle_event (&ev);
	  break;

	case SDL_QUIT:
	  game_quit ();
	}
    }
}

void
game_quit (void)
{
  sound_finalise ();
  display_finalise ();
  SDL_Quit ();
  exit (0);
}

void
game_transform_path_full (const char *file_name, char *buffer, size_t size)
{
  if (strncasecmp (file_name, SAVEGAME_PATH,
		   sizeof (SAVEGAME_PATH) - 1) == 0)
    {
      snprintf (buffer, size, "%s" FS_SEP_STR "%s", GetDirectoryUser(),
		file_name + sizeof (SAVEGAME_PATH) - 1);
      return;
    }

  /* abort on absolute paths */
  if (file_name[0] == '\\' || file_name[0] == '/'
      || (strlen (file_name) >= 2 && file_name[1] == ':'))
    {
      snprintf (buffer, size, "%s", file_name);
      return;
    }

  snprintf (buffer, size, "%s" FS_SEP_STR "%s", GetDirectoryHdd(), file_name);
}

void
game_transform_path (const char *file_name, char *result)
{
  game_transform_path_full (file_name, result, FILENAME_MAX);
}

void
game_play_music (void)
{
  char file_name[FILENAME_MAX];

  snprintf (file_name, sizeof (file_name),
	    "%s" FS_SEP_STR "music" FS_SEP_STR "track_%i.ogg",
	    GetDirectoryHdd(), game_music_track - 1);

  sound_open_music_file (file_name);
  sound_play_music ();
}

static void
game_update_full (bool wait)
{
  const int max_fps = 16;
  const int32_t frame_duration = 1000 / max_fps;

  static int32_t last_frame_ticks;
  int32_t start_ticks;

  display_unlock ();

  sound_update ();
  display_update ();
  game_handle_sdl_events ();

  start_ticks = SDL_GetTicks ();

  if (wait && last_frame_ticks != 0)
    {
      int32_t last_frame_duration;

      last_frame_duration = (int32_t) (start_ticks - last_frame_ticks + 2);

      if (last_frame_duration < frame_duration)
	{
	  int32_t total_sleep_time = frame_duration - last_frame_duration;
	  const int32_t min_sleep_time = 1000 / 40;
	  const int32_t max_sleep_time = 1000 / 20;

	  total_sleep_time = frame_duration - last_frame_duration;

	  if (total_sleep_time > 0)
	    {
	      float f = (float) total_sleep_time
			* (min_sleep_time + max_sleep_time)
			/ (2 * min_sleep_time * max_sleep_time);
	      int32_t base_sleep_time = (int32_t) (total_sleep_time / f + .5f);

	      while (total_sleep_time > 0)
		{
		  int32_t sleep_time = MIN (base_sleep_time, total_sleep_time);
		  int32_t ticks = SDL_GetTicks ();

		  SDL_Delay (sleep_time);

		  display_lock ();
		  game_handle_sdl_events ();
		  sound_update ();
		  display_unlock ();

		  display_update_mouse_pointer ();

		  total_sleep_time -= SDL_GetTicks () - ticks;
		}
	    }
	}
    }

  last_frame_ticks = SDL_GetTicks ();

  display_lock ();
}

int
game_wait_for_vsync (void)
{
  game_update_full (false);
  return 1;
}

void
game_update (void)
{
  game_update_full (true);
}
