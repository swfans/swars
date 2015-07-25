#include <stdbool.h>
#include <stdint.h>

#include "display.h"
#include "mouse.h"

extern uint32_t	mouse_installed;
extern int32_t	mouse_x;
extern int32_t	mouse_y;
extern int32_t	mouse_x_delta;
extern int32_t	mouse_y_delta;
extern int32_t	mouse_press_x;
extern int32_t	mouse_press_y;
extern int32_t	mouse_release_x;
extern int32_t	mouse_release_y;
extern bool	mouse_left_pressed;
extern bool	mouse_middle_pressed;
extern bool	mouse_right_pressed;
extern bool	mouse_left_press_locked;
extern bool	mouse_right_press_locked;
extern bool	mouse_middle_press_locked;
extern bool	mouse_left_release_locked;
extern bool	mouse_middle_release_locked;
extern bool	mouse_right_release_locked;

static void
transform_mouse (int32_t *x, int32_t *y)
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
      mouse_press_x = ev->x;
      mouse_press_y = ev->y;
      transform_mouse (&mouse_press_x, &mouse_press_y);
    }
  else
    {
      mouse_release_x = ev->x;
      mouse_release_y = ev->y;
      transform_mouse (&mouse_release_x, &mouse_release_y);
    }
}

static void
handle_button_event (const SDL_MouseButtonEvent *ev)
{
  if (ev->type == SDL_MOUSEBUTTONDOWN)
    {
      if (ev->button == SDL_BUTTON_LEFT)
	{
	  mouse_left_pressed = true;

	  if (!mouse_left_press_locked)
	    {
	      mouse_left_press_locked = true;
	      mouse_left_release_locked = false;
	      store_button_coordinates (ev);
	    }
	}
      else if (ev->button == SDL_BUTTON_MIDDLE)
	{
	  mouse_middle_pressed = true;

	  if (!mouse_middle_press_locked)
	    {
	      mouse_middle_press_locked = true;
	      mouse_middle_release_locked = false;
	      store_button_coordinates (ev);
	    }
	}
      else if (ev->button == SDL_BUTTON_RIGHT)
	{
	  mouse_right_pressed = true;

	  if (!mouse_right_press_locked)
	    {
	      mouse_right_press_locked = true;
	      mouse_right_release_locked = false;
	      store_button_coordinates (ev);
	    }
	}
    }
  else
    {
      if (ev->button == SDL_BUTTON_LEFT)
	{
	  mouse_left_pressed = false;

	  if (!mouse_left_release_locked)
	    {
	      mouse_left_release_locked = true;
	      store_button_coordinates (ev);
	    }
	}
      else if (ev->button == SDL_BUTTON_MIDDLE)
	{
	  mouse_middle_pressed = false;

	  if (!mouse_middle_release_locked)
	    {
	      mouse_middle_release_locked = true;
	      store_button_coordinates (ev);
	    }
	}
      else if (ev->button == SDL_BUTTON_RIGHT)
	{
	  mouse_right_pressed = false;

	  if (!mouse_right_release_locked)
	    {
	      mouse_right_release_locked = true;
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
  mouse_x = ev->x;
  mouse_y = ev->y;
  transform_mouse (&mouse_x, &mouse_y);
  transform_mouse (&mouse_x_delta, &mouse_y_delta);

  asm volatile
    ("call mouse_correct_to_within_bounds;"
     "call func_e9e58;"
     "call func_e9ba0"
     : : "a" (&mouse_x), "d" (&mouse_y));

}

void
mouse_handle_event (const SDL_Event *ev)
{
  if (!mouse_installed)
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

