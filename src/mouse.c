#include <stdbool.h>
#include <stdint.h>

#include "display.h"
#include "mouse.h"
#include "bflib_video.h"

#pragma pack(1)

extern long	lbMouseInstalled;
extern long	mouse_x_delta;
extern long	mouse_y_delta;

#pragma pack()

static void
transform_mouse (long *x, long *y)
{
  size_t phys_x, phys_y;
  size_t disp_x, disp_y;

  if (!display_is_stretching_enabled ())
    return;

  display_get_physical_size (&phys_x, &phys_y);
  display_get_size (&disp_x, &disp_y);

  /*
   * The "+ 1" shouldn't be needed, but for some reason on Mac OS X in 640x480
   * you can only move your cusror as far as y=477, which prevents you from
   * reaching the bottom edge of the screen after the transformation.
   *
   * On other systems, where you can reach y=479, this can result in y=200 after
   * the transformation, but the game has no problem with that.
   */
  *x = ((*x) * (ssize_t) (disp_x + 1)) / (ssize_t) phys_x;
  *y = ((*y) * (ssize_t) (disp_y + 1)) / (ssize_t) phys_y;
}

static void
store_button_coordinates (const SDL_MouseButtonEvent *ev)
{
  if (ev->type == SDL_MOUSEBUTTONDOWN)
    {
      lbDisplay.MouseX = ev->x;
      lbDisplay.MouseY = ev->y;
      transform_mouse (&lbDisplay.MouseX, &lbDisplay.MouseY);
    }
  else
    {
      lbDisplay.RMouseX = ev->x;
      lbDisplay.RMouseY = ev->y;
      transform_mouse (&lbDisplay.RMouseX, &lbDisplay.RMouseY);
    }
}

static void
handle_button_event (const SDL_MouseButtonEvent *ev)
{
  if (ev->type == SDL_MOUSEBUTTONDOWN)
    {
      if (ev->button == SDL_BUTTON_LEFT)
	{
	  lbDisplay.MLeftButton = true;

	  if (!lbDisplay.LeftButton)
	    {
	      lbDisplay.LeftButton = true;
	      lbDisplay.RLeftButton = false;
	      store_button_coordinates (ev);
	    }
	}
      else if (ev->button == SDL_BUTTON_MIDDLE)
	{
	  lbDisplay.MMiddleButton = true;

	  if (!lbDisplay.MiddleButton)
	    {
	      lbDisplay.MiddleButton = true;
	      lbDisplay.RMiddleButton = false;
	      store_button_coordinates (ev);
	    }
	}
      else if (ev->button == SDL_BUTTON_RIGHT)
	{
	  lbDisplay.MRightButton = true;

	  if (!lbDisplay.RightButton)
	    {
	      lbDisplay.RightButton = true;
	      lbDisplay.RRightButton = false;
	      store_button_coordinates (ev);
	    }
	}
    }
  else
    {
      if (ev->button == SDL_BUTTON_LEFT)
	{
	  lbDisplay.MLeftButton = false;

	  if (!lbDisplay.RLeftButton)
	    {
	      lbDisplay.RLeftButton = true;
	      store_button_coordinates (ev);
	    }
	}
      else if (ev->button == SDL_BUTTON_MIDDLE)
	{
	  lbDisplay.MMiddleButton = false;

	  if (!lbDisplay.RMiddleButton)
	    {
	      lbDisplay.RMiddleButton = true;
	      store_button_coordinates (ev);
	    }
	}
      else if (ev->button == SDL_BUTTON_RIGHT)
	{
	  lbDisplay.MRightButton = false;

	  if (!lbDisplay.RRightButton)
	    {
	      lbDisplay.RRightButton = true;
	      store_button_coordinates (ev);
	    }
	}
    }
}

static void
handle_motion_event (const SDL_MouseMotionEvent *ev)
{
  mouse_x_delta = ev->xrel;
  mouse_y_delta = ev->yrel;
  lbDisplay.MMouseX = ev->x;
  lbDisplay.MMouseY = ev->y;
  transform_mouse (&lbDisplay.MMouseX, &lbDisplay.MMouseY);
  transform_mouse (&mouse_x_delta, &mouse_y_delta);

  asm volatile
    ("call adjust_point;"
     "call screen_remove;"
     "call func_e9ba0"
     : : "a" (&lbDisplay.MMouseX), "d" (&lbDisplay.MMouseY));

}

void
mouse_handle_event (const SDL_Event *ev)
{
  if (!lbMouseInstalled)
    return;

  switch (ev->type)
    {
    case SDL_MOUSEMOTION:
      handle_motion_event (&ev->motion);
      break;

    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN:
      handle_button_event (&ev->button);
      break;
    }
}

void
mouse_initialise (void)
{
  SDL_ShowCursor (SDL_DISABLE);
}

