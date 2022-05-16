#include <stdint.h>
#include <stdio.h>

#include "display.h"
#include "bfscreen.h"
#include "bfscrsurf.h"
#include "bftext.h"
#include "bfmouse.h"
#include "util.h"
#include "bflib_basics.h"

#pragma pack(1)

#define to_SDLSurf(h) ((SDL_Surface  *)h)

extern TbScreenModeInfo lbScreenModeInfo[];

// Misc stuff
extern uint32_t screen_initialised;

// Drawing of the mouse cursor
extern int32_t mbuffer__X;
extern int32_t mbuffer__Y;
extern int32_t mbuffer__Width;
extern int32_t mbuffer__Height;

#pragma pack()

static bool	    display_full_screen = false;
static bool         display_lowres_stretch = false;
static unsigned char *display_stretch_buffer = NULL;
static SDL_Color    display_palette[256];

TbResult LbScreenSetGraphicsWindow_TODEL(ulong x, ulong y, ulong width, ulong height)
{
  TbResult ret;
  asm volatile
    ("call ASM_LbScreenSetGraphicsWindow"
     : "=r" (ret) : "a" (x), "d" (y), "b" (width), "c" (height));
  return ret;
}

TbResult LbTextSetWindow_TODEL(ulong x, ulong y, ulong width, ulong height)
{
  TbResult ret;
  asm volatile
    ("call ASM_LbTextSetWindow"
     : "=r" (ret) : "a" (x), "d" (y), "b" (width), "c" (height));
  return ret;
}

static inline void
lock_screen (void)
{
  if (!SDL_MUSTLOCK (to_SDLSurf(lbDrawSurface)))
    return;

  if (SDL_LockSurface (to_SDLSurf(lbDrawSurface)) != 0)
    fprintf (stderr, "SDL_LockSurface: %s\n", SDL_GetError ());
}

static inline void
unlock_screen (void)
{
  if (!SDL_MUSTLOCK (to_SDLSurf(lbDrawSurface)))
    return;

  SDL_UnlockSurface (to_SDLSurf(lbDrawSurface));
}

TbResult
LbPaletteSet(const unsigned char *palette)
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

  if (SDL_SetPalette (to_SDLSurf(lbDrawSurface),
		      SDL_LOGPAL | SDL_PHYSPAL, colours, 0, 256) != 1)
    {
      fprintf (stderr, "SDL_SetPalette: %s\n", SDL_GetError ());
      return -1;
    }

  memcpy (display_palette, colours, sizeof (display_palette));

  return 1;
}

int LbScreenSetupAnyMode(unsigned short mode, unsigned long width,
    unsigned long height, TbPixel *palette)
{
  uint32_t flags;

  LbMouseSuspend();

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
  if (to_SDLSurf(lbDrawSurface) != NULL)
    {
      unlock_screen ();
      SDL_FreeSurface (to_SDLSurf(lbDrawSurface));
    }

  flags = SDL_SWSURFACE;

  if (display_full_screen)
    flags |= SDL_FULLSCREEN;

  // Stretch lowres ?
  if (width == 320 && height == 200 && display_lowres_stretch)
    {
      // Init mode
      lbScreenSurface = lbDrawSurface =
          SDL_SetVideoMode (640, 480,
  		    lbScreenModeInfo[mode].BitsPerPixel, flags);

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
      lbScreenSurface = lbDrawSurface =
          SDL_SetVideoMode (width, height,
 		    lbScreenModeInfo[mode].BitsPerPixel, flags);
    }


#ifdef DEBUG
  printf ("SDL_SetVideoMode(%ld, %ld, %d, SDL_SWSURFACE) - %s\n",
          width, height, (int)lbScreenModeInfo[mode].BitsPerPixel,
          lbScreenModeInfo[mode].Desc);
#endif

  if (lbDrawSurface == NULL)
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
      lbDisplay.PhysicalScreen = to_SDLSurf(lbDrawSurface)->pixels;
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
  LbScreenSetGraphicsWindow(0, 0, lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);
  LbTextSetWindow(0, 0, lbDisplay.GraphicsScreenWidth, lbDisplay.GraphicsScreenHeight);

  // Setup palette
  if (palette != NULL)
    {
      if (LbPaletteSet(palette) != 1)
	goto err;
    }

  screen_initialised = true;

  return 1;

err:
  if (lbDrawSurface != NULL)
    {
      unlock_screen ();
      SDL_FreeSurface (to_SDLSurf(lbDrawSurface));
      lbDrawSurface = NULL;
    }

  if (display_stretch_buffer)
    {
      xfree (display_stretch_buffer);
      display_stretch_buffer = NULL;
    }

  screen_initialised = false;

  return -1;
}

/* FIXME: doesn't work with stretching */
void
display_update_mouse_pointer (void)
{
  int x, y;

  x = MAX (0, mbuffer__X);
  y = MAX (0, mbuffer__Y);

  SDL_UpdateRect (to_SDLSurf(lbDrawSurface), x, y,
		  mbuffer__Width, mbuffer__Height);
}

void
display_update (void)
{
  // Stretched lowres in action?
  if (display_stretch_buffer != NULL)
    {
      // Stretch lowres
      int i, j;
      unsigned char *poutput = (unsigned char*) to_SDLSurf(lbDrawSurface)->pixels;
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

  SDL_Flip (to_SDLSurf(lbDrawSurface));
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
  SDL_FreeSurface (to_SDLSurf(lbDrawSurface));
  lbDrawSurface = NULL;
  lbDisplay.PhysicalScreen = NULL;
}

void
display_set_full_screen (bool full_screen)
{
  if (lbDrawSurface != NULL)
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
  if (lbDisplay.PhysicalScreen == NULL || lbDrawSurface == NULL)
    {
      if (width != NULL)
        *width  = 0;

      if (height != NULL)
        *height = 0;

      return;
    }

  if (width != NULL)
    *width  = to_SDLSurf(lbDrawSurface)->w;

  if (height != NULL)
    *height = to_SDLSurf(lbDrawSurface)->h;
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
