#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <png.h>

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

static char *game_user_path;
static char *game_data_path;

static bool
create_user_directory (void)
{
  char buffer[FILENAME_MAX];

  if (!sys_get_user_path (buffer, sizeof (buffer)))
    {
      game_user_path = xstrdup ("qdata" FS_SEP_STR "savegame");
      return true;
    }

  game_user_path = xstrdup (buffer);

  return mkdir_recursive (game_user_path, 0700);
}

bool
game_initialise (void)
{
  char buffer[FILENAME_MAX];

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

  if (sys_get_data_path (buffer, sizeof (buffer)))
    game_data_path = xstrdup (buffer);
  else
    game_data_path = xstrdup (".");

  create_user_directory ();

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
  xfree (game_user_path);
  xfree (game_data_path);
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
      snprintf (buffer, size, "%s" FS_SEP_STR "%s", game_user_path,
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

  snprintf (buffer, size, "%s" FS_SEP_STR "%s", game_data_path, file_name);
}

void
game_transform_path (const char *file_name, char *result)
{
  game_transform_path_full (file_name, result, FILENAME_MAX);
}

const char *
game_get_data_path (void)
{
  return game_data_path;
}

const char *
game_get_user_path (void)
{
  return game_user_path;
}

ssize_t
game_file_length (const char *filename)
{
  struct stat st;
  char transformed[FILENAME_MAX];
  char path[FILENAME_MAX];

  game_transform_path_full (filename, transformed, sizeof (transformed));
  dos_path_to_native (transformed, path, sizeof (path));

  if (stat (path, &st) != 0)
    {
      perror (path);
      return -1;
    }

  return st.st_size;
}

static bool
game_make_png_screenshot (const char *file_name, const uint8_t *frame_buffer,
			  const uint8_t *game_palette)
{
  FILE *f;
  png_structp png = NULL;
  png_infop info = NULL;
  png_color palette[256];
  png_byte **rows;
  size_t w, h, i;
  char *action;

  // get parameters
  display_get_size (&w, &h);
  rows = (png_byte**) xmalloc (sizeof (png_byte*) * h);

  // set image rows for png processing
  for (i = 0; i < h; i++)
    rows[i] = (png_byte *) frame_buffer + i * w;

  // create file
  action = "create file";
  f = fopen (file_name, "wb");
  if (f == NULL)
    {
      perror (file_name);
      goto err;
    }

  // initialise
  action = "create png_create_write_struct";
  png = png_create_write_struct (PNG_LIBPNG_VER_STRING, 0, 0, 0);
  if (png == NULL)
    goto err;

  action = "create png_create_info_struct";
  info = png_create_info_struct (png);
  if (info == NULL)
    goto err;

  png_init_io (png, f);

  // write header
  png_set_IHDR (png, info, w, h, 8,
                PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_ADAM7,
                PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  for (i = 0; i < 256; i++)
    {
      palette[i].red   = game_palette[3 * i + 0] * 4;
      palette[i].green = game_palette[3 * i + 1] * 4;
      palette[i].blue  = game_palette[3 * i + 2] * 4;
    }

  png_set_PLTE (png, info, palette, 256);
  png_write_info (png, info);

  // write bytes
  png_write_image (png, rows);

  // finalise writing
  png_write_end (png, info);
  // cleanup heap allocation
  png_destroy_write_struct (&png, &info);
  xfree (rows);
  fclose (f);
  return true;

err: // handle error and cleanup heap allocation
  fprintf (stderr, "Could not %s (%s) !\n", action, file_name);
  if (png != NULL && info != NULL)
    png_destroy_write_struct (&png, &info);
  else if (png != NULL)
    png_destroy_write_struct (&png, (png_infopp) NULL);
  xfree (rows);
  fclose (f);
  return false;
}

void
game_make_screenshot (const char *id, const uint8_t *frame_buffer,
		      const uint8_t *palette)
{
  char directory[FILENAME_MAX];
  char file_name[FILENAME_MAX];
  struct stat st;
  int n;

  snprintf (directory, sizeof (directory), "%s" FS_SEP_STR "screenshots",
	    game_get_user_path ());

  if (!mkdir_recursive (directory, 0755))
    return;

  for (n = 1; n < INT_MAX; n++)
    {
      snprintf (file_name, sizeof (file_name),
		"%s" FS_SEP_STR "shot%04i.png", directory, n);

      if (stat (file_name, &st) == 0)
	continue;

      if (errno != ENOENT)
	{
	  perror (file_name);
	  return;
	}

      game_make_png_screenshot (file_name, frame_buffer, palette);
      return;
    }
}

void
game_play_music (void)
{
  char file_name[FILENAME_MAX];

  snprintf (file_name, sizeof (file_name),
	    "%s" FS_SEP_STR "music" FS_SEP_STR "track_%i.ogg",
	    game_data_path, game_music_track - 1);

  sound_open_music_file (file_name);
  sound_play_music ();
}

static void
game_update_full (bool wait)
{
  const int max_fps = 22;
  const int32_t frame_duration = 1000 / max_fps;

  static int32_t last_frame_ticks;
  int32_t start_ticks;
  int32_t ticks;

  display_unlock ();

  sound_update ();
  display_update ();
  game_handle_sdl_events ();

  start_ticks = SDL_GetTicks ();

  if (wait && last_frame_ticks != 0)
    {
      int32_t last_frame_duration;

      last_frame_duration = (int32_t) (start_ticks - last_frame_ticks);

      if (last_frame_duration < frame_duration)
	{
	  int32_t total_sleep_time;
	  int32_t sleep_time;
	  const int32_t min_sleep_time = 1000 / 40;
	  const int32_t max_sleep_time = 1000 / 20;

	  total_sleep_time = frame_duration - last_frame_duration;

	  while (total_sleep_time > 0)
	    {
	      ticks = SDL_GetTicks ();

	      sleep_time = MAX (total_sleep_time, min_sleep_time);
	      sleep_time = MIN (sleep_time, max_sleep_time);
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
