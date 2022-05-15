#include <stdbool.h>
#include <stdint.h>

#include "mouse.h"
#include "bfscreen.h"
#include "bfplanar.h"
#include "bfmouse.h"
#include "display.h"


void transform_mouse (struct TbPoint *pos)
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
  pos->x = ((pos->x) * (ssize_t) (disp_x + 1)) / (ssize_t) phys_x;
  pos->y = ((pos->y) * (ssize_t) (disp_y + 1)) / (ssize_t) phys_y;
}

TbResult
mouseControl(TbMouseAction action, struct TbPoint *pos)
{
    if (action == MActn_MOUSEMOVE)
	{
      transform_mouse (pos);
      lbDisplay.MMouseX = pos->x;
      lbDisplay.MMouseY = pos->y;

      asm volatile
        ("call adjust_point;"
         "call screen_remove;"
         "call screen_place"
         : : "a" (&lbDisplay.MMouseX), "d" (&lbDisplay.MMouseY));
	} else
      if (action == MActn_LBUTTONDOWN)
	{
	  lbDisplay.MLeftButton = true;

	  if (!lbDisplay.LeftButton)
	    {
      transform_mouse (pos);
	      lbDisplay.LeftButton = true;
	      lbDisplay.RLeftButton = false;
          lbDisplay.MouseX = pos->x;
          lbDisplay.MouseY = pos->y;
	    }
	}
      else if (action == MActn_MBUTTONDOWN)
	{
	  lbDisplay.MMiddleButton = true;

	  if (!lbDisplay.MiddleButton)
	    {
      transform_mouse (pos);
	      lbDisplay.MiddleButton = true;
	      lbDisplay.RMiddleButton = false;
          lbDisplay.MouseX = pos->x;
          lbDisplay.MouseY = pos->y;
	    }
	}
      else if (action == MActn_RBUTTONDOWN)
	{
	  lbDisplay.MRightButton = true;

	  if (!lbDisplay.RightButton)
	    {
      transform_mouse (pos);
	      lbDisplay.RightButton = true;
	      lbDisplay.RRightButton = false;
          lbDisplay.MouseX = pos->x;
          lbDisplay.MouseY = pos->y;
	    }
	} else

      if (action == MActn_LBUTTONUP)
	{
	  lbDisplay.MLeftButton = false;

	  if (!lbDisplay.RLeftButton)
	    {
          transform_mouse (pos);
	      lbDisplay.RLeftButton = true;
          lbDisplay.RMouseX = pos->x;
          lbDisplay.RMouseY = pos->y;
	    }
	}
      else if (action == MActn_MBUTTONUP)
	{
	  lbDisplay.MMiddleButton = false;

	  if (!lbDisplay.RMiddleButton)
	    {
          transform_mouse (pos);
	      lbDisplay.RMiddleButton = true;
          lbDisplay.RMouseX = pos->x;
          lbDisplay.RMouseY = pos->y;
	    }
	}
      else if (action == MActn_RBUTTONUP)
	{
	  lbDisplay.MRightButton = false;

	  if (!lbDisplay.RRightButton)
	    {
          transform_mouse (pos);
	      lbDisplay.RRightButton = true;
          lbDisplay.RMouseX = pos->x;
          lbDisplay.RMouseY = pos->y;
	    }
	}
    return Lb_OK;
}

void
mouse_initialise (void)
{
  SDL_ShowCursor (SDL_DISABLE);
}

