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

  MouseToScreen(pos);

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
    struct TbPoint dstPos;

    if (!lbMouseInstalled)
        return Lb_OK;
    dstPos.x = pos->x;
    dstPos.y = pos->y;

    if (action == MActn_MOUSEMOVE)
	{
      lbMouseToScreen(&dstPos);
      lbDisplay.MMouseX = dstPos.x;
      lbDisplay.MMouseY = dstPos.y;

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
          lbMouseToScreen(&dstPos);
	      lbDisplay.LeftButton = true;
	      lbDisplay.RLeftButton = false;
          lbDisplay.MouseX = dstPos.x;
          lbDisplay.MouseY = dstPos.y;
	    }
	}
      else if (action == MActn_MBUTTONDOWN)
	{
	  lbDisplay.MMiddleButton = true;

	  if (!lbDisplay.MiddleButton)
	    {
          lbMouseToScreen(&dstPos);
	      lbDisplay.MiddleButton = true;
	      lbDisplay.RMiddleButton = false;
          lbDisplay.MouseX = dstPos.x;
          lbDisplay.MouseY = dstPos.y;
	    }
	}
      else if (action == MActn_RBUTTONDOWN)
	{
	  lbDisplay.MRightButton = true;

	  if (!lbDisplay.RightButton)
	    {
          lbMouseToScreen(&dstPos);
	      lbDisplay.RightButton = true;
	      lbDisplay.RRightButton = false;
          lbDisplay.MouseX = dstPos.x;
          lbDisplay.MouseY = dstPos.y;
	    }
	} else

      if (action == MActn_LBUTTONUP)
	{
	  lbDisplay.MLeftButton = false;

	  if (!lbDisplay.RLeftButton)
	    {
          lbMouseToScreen(&dstPos);
	      lbDisplay.RLeftButton = true;
          lbDisplay.RMouseX = dstPos.x;
          lbDisplay.RMouseY = dstPos.y;
	    }
	}
      else if (action == MActn_MBUTTONUP)
	{
	  lbDisplay.MMiddleButton = false;

	  if (!lbDisplay.RMiddleButton)
	    {
          lbMouseToScreen(&dstPos);
	      lbDisplay.RMiddleButton = true;
          lbDisplay.RMouseX = dstPos.x;
          lbDisplay.RMouseY = dstPos.y;
	    }
	}
      else if (action == MActn_RBUTTONUP)
	{
	  lbDisplay.MRightButton = false;

	  if (!lbDisplay.RRightButton)
	    {
          lbMouseToScreen(&dstPos);
	      lbDisplay.RRightButton = true;
          lbDisplay.RMouseX = dstPos.x;
          lbDisplay.RMouseY = dstPos.y;
	    }
	}
    return Lb_OK;
}

void
mouse_initialise (void)
{
  SDL_ShowCursor (SDL_DISABLE);
}

