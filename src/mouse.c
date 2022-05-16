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

  if (!display_is_stretching_enabled())
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

void mouse_initialise(void)
{
    SDL_ShowCursor(SDL_DISABLE);
}

