#include <stdint.h>
#include <stdio.h>

#include "display.h"
#include "util.h"
#include "bflib_video.h"

#pragma pack(1)

// Internal mode list
/*struct ScreenModeInfo
{
  uint16_t width;      // e.g. 320
  uint16_t height;     // e.g. 200
  uint8_t  bpp;        // e.g. 8
  uint8_t  padding;
  uint32_t noidea1;
  uint32_t mode;       // e.g. 13h
  char     name[0x18]; // e.g. MODE_320_200_8
} __attribute__((packed));

typedef struct ScreenModeInfo TbScreenModeInfo;*/

extern TbScreenModeInfo lbScreenModeInfo[];

// Misc stuff
extern uint32_t display_good;

// Drawing of the mouse cursor
extern int32_t mouse_pointer_x;
extern int32_t mouse_pointer_y;
extern int32_t mouse_pointer_width;
extern int32_t mouse_pointer_height;

#pragma pack()

static SDL_Surface *display_screen;
static bool	    display_full_screen = false;
static bool         display_lowres_stretch = false;
static unsigned char *display_stretch_buffer = NULL;
static SDL_Color    display_palette[256];

static void
call_e0d00 (int x, int y, int w, int h)
{
  asm volatile
    ("push %%ebx;"
     "mov  %2,%%ebx;"
     "call LbScreenSetGraphicsWindow_;"
     "pop  %%ebx"
     : : "a" (x), "d" (y), "g" (w), "c" (h));
}

static void
call_ef4f0 (int x, int y, int w, int h)
{
  asm volatile
    ("push %%ebx;"
     "mov  %2,%%ebx;"
     "call LbTextSetWindow_;"
     "pop  %%ebx"
     : : "a" (x), "d" (y), "g" (w), "c" (h));
}

// call e9498 aka display_set_mode_setup_mouse
static void
call_display_set_mode_setup_mouse (void)
{
  asm volatile ("call display_set_mode_setup_mouse");
}

static inline void
lock_screen (void)
{
  if (!SDL_MUSTLOCK (display_screen))
    return;

  if (SDL_LockSurface (display_screen) != 0)
    fprintf (stderr, "SDL_LockSurface: %s\n", SDL_GetError ());
}

static inline void
unlock_screen (void)
{
  if (!SDL_MUSTLOCK (display_screen))
    return;

  SDL_UnlockSurface (display_screen);
}

int
display_set_palette (const uint8_t *palette)
{
  SDL_Color colours[256];
  int n;

  for (n = 0; n < 256; n++)
    {
      colours[n].r = palette[3 * n + 0] * 4;
      colours[n].g = palette[3 * n + 1] * 4;
      colours[n].b = palette[3 * n + 2] * 4;
      colours[n].unused = 0;
    }

  if (SDL_SetPalette (display_screen,
		      SDL_LOGPAL | SDL_PHYSPAL, colours, 0, 256) != 1)
    {
      fprintf (stderr, "SDL_SetPalette: %s\n", SDL_GetError ());
      return -1;
    }

  memcpy (display_palette, colours, sizeof (display_palette));

  return 1;
}

int
display_set_mode (uint16_t mode, uint32_t width, uint32_t height,
		  const uint8_t *palette)
{
  uint32_t flags;

  // call func_e9498
  call_display_set_mode_setup_mouse ();

  // lbDisplay.OldVideoMode which is DWORD 1E2EB6 is used in
  // 000ED764 sub_ED764 to probably get back to text mode
  // I'm setting it to 0xFF for now
  if (!lbDisplay.OldVideoMode)
    lbDisplay.OldVideoMode = 0xFF;

  // We are using only lbDisplay.VesaIsSetUp = false

  /*  if(lbScreenModeInfo[mode].mode < 256)
  {
    lbDisplay.VesaIsSetUp = false;
  }
  else
  {
    // lbDisplay.VesaIsSetUp = true;
    lbDisplay.VesaIsSetUp = false; // Defaulting to false
  }*/
  lbDisplay.VesaIsSetUp = false;

  // Setting mode
  if (display_screen != NULL)
    {
      unlock_screen ();
      SDL_FreeSurface (display_screen);
    }

  flags = SDL_SWSURFACE;

  if (display_full_screen)
    flags |= SDL_FULLSCREEN;

  // Stretch lowres ?
  if (width == 320 && height == 200 && display_lowres_stretch)
    {
      // Init mode
      display_screen = SDL_SetVideoMode (640, 480,
  				         lbScreenModeInfo[mode].BitsPerPixel,
                                         flags);

      // Allocate buffer
      if (display_stretch_buffer == NULL)
        {
          display_stretch_buffer = xmalloc(320 * 240);
        }
    }
  else
    {
      // Remove buffer if any
      if (display_stretch_buffer != NULL)
        {
          xfree (display_stretch_buffer);
          display_stretch_buffer = NULL;
        }

      // Init mode
      display_screen = SDL_SetVideoMode (width, height,
 				         lbScreenModeInfo[mode].BitsPerPixel,
				         flags);
    }


#ifdef DEBUG
  printf ("SDL_SetVideoMode(%i, %i, %i, SDL_SWSURFACE) - %s\n",
          width, height, lbScreenModeInfo[mode].BitsPerPixel,
          lbScreenModeInfo[mode].Desc);
#endif

  if (display_screen == NULL)
    {
      fprintf (stderr, "SDL_SetVideoMode: %s\n", SDL_GetError ());
      goto err;
    }

  lock_screen ();

  // set vga buffer address
  if (display_stretch_buffer != NULL)
    {
      // Set the temporary buffer
      lbDisplay.PhysicalScreen = display_stretch_buffer;
    }
  else
    {
      // Set the good buffer
      lbDisplay.PhysicalScreen = display_screen->pixels;
    }

  // Setup some global variables
  lbDisplay.PhysicalScreenWidth  = lbScreenModeInfo[mode].Width;
  lbDisplay.PhysicalScreenHeight = lbScreenModeInfo[mode].Height;
  lbDisplay.GraphicsScreenWidth  = width;
  lbDisplay.GraphicsScreenHeight = height;
  lbDisplay.ScreenMode   = mode;

  // No idea what is this
  // TODO: check if something breaks if this is removed
  lbDisplay.DrawFlags = 0;
  lbDisplay.DrawColour = 0;

  // Call funcitons that recalculate some buffers
  // They can be switched to C++ later, but it's not needed
  call_e0d00 (0, 0, lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);
  call_ef4f0 (0, 0, lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);

  // Setup palette
  if (palette != NULL)
    {
      if (display_set_palette(palette) != 1)
	goto err;
    }

  display_good = true;

  return 1;

err:
  if (display_screen != NULL)
    {
      unlock_screen ();
      SDL_FreeSurface (display_screen);
      display_screen = NULL;
    }

  if (display_stretch_buffer)
    {
      xfree (display_stretch_buffer);
      display_stretch_buffer = NULL;
    }

  display_good = false;

  return -1;
}

/* FIXME: doesn't work with stretching */
void
display_update_mouse_pointer (void)
{
  int x, y;

  x = MAX (0, mouse_pointer_x);
  y = MAX (0, mouse_pointer_y);

  SDL_UpdateRect (display_screen, x, y,
		  mouse_pointer_width, mouse_pointer_height);
}

void
display_update (void)
{
  // Stretched lowres in action?
  if (display_stretch_buffer != NULL)
    {
      // Stretch lowres
      int i, j;
      unsigned char *poutput = (unsigned char*) display_screen->pixels;
      unsigned char *pinput  = display_stretch_buffer;

      for (j = 0; j < 480; j++)
        {
          for (i = 0; i < 640; i+=2)
            {
              // Do not touch this formula
              int input_xy = ((j * 200) / 480) * 320 + i / 2;
              int output_xy = j * 640 + i;

              poutput[output_xy]     = pinput[input_xy];
              poutput[output_xy + 1] = pinput[input_xy];
            }
        }
  }

  SDL_Flip (display_screen);
}

void
display_initialise (void)
{
  SDL_WM_SetCaption ("Syndicate Wars", NULL);
}

void
display_finalise (void)
{
  unlock_screen ();
  SDL_FreeSurface (display_screen);
  display_screen = NULL;
  lbDisplay.PhysicalScreen = NULL;
}

void
display_set_full_screen (bool full_screen)
{
  if (display_screen != NULL)
    return;

  display_full_screen = full_screen;
}

void
display_get_size (size_t *width, size_t *height)
{
  if (lbDisplay.PhysicalScreen == NULL)
    {
      if (width != NULL)
        *width  = 0;

      if (height != NULL)
        *height = 0;

      return;
    }

  if (width != NULL)
    *width  = lbDisplay.GraphicsScreenWidth;

  if (height != NULL)
    *height = lbDisplay.GraphicsScreenHeight;
}

void
display_get_physical_size (size_t *width, size_t *height)
{
  if (lbDisplay.PhysicalScreen == NULL || display_screen == NULL)
    {
      if (width != NULL)
        *width  = 0;

      if (height != NULL)
        *height = 0;

      return;
    }

  if (width != NULL)
    *width  = display_screen->w;

  if (height != NULL)
    *height = display_screen->h;
}

void *
display_get_buffer (void)
{
  return lbDisplay.PhysicalScreen;
}

void
display_get_palette (SDL_Color *colours)
{
  memcpy (colours, display_palette, sizeof (display_palette));
}

void
display_set_lowres_stretch (bool stretch)
{
  display_lowres_stretch = stretch;
}

bool
display_is_stretching_enabled (void)
{
  return (bool) (display_stretch_buffer != NULL);
}

void
display_lock (void)
{
  lock_screen ();
}

void
display_unlock (void)
{
  unlock_screen ();
}
