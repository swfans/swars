#include <SDL.h>
#include <assert.h>
#include <stdint.h>

#include "timer.h"

#include "sound.h"

#pragma pack(1)

extern TimerCallback timer_callback[TIMER_MAX_CALLBACKS];
extern uint32_t	     timer_status[TIMER_MAX_CALLBACKS];
extern void         *timer_user[TIMER_MAX_CALLBACKS];

#pragma pack()

uint32_t
timer_get_18_2_hz_ticks (void)
{
  return (((uint64_t) SDL_GetTicks () * 1193182 / 1000) >> 15);
}

uint32_t
MSSSMACKTIMERREAD (void)
{
  return SDL_GetTicks ();
}

TimerId
timer_register_callback (TimerCallback fn)
{
  size_t n;

  for (n = 0; n < TIMER_MAX_CALLBACKS - 1; n++)
    {
      if (timer_status[n] != 0)
	continue;

      timer_status[n] = 1;
      timer_callback[n] = fn;

      return (4 * n);
    }

  return -1;
}

void *
timer_set_user_data (TimerId timer, void *user_data)
{
  void *retval;

  assert (timer >= 0);
  assert (timer % sizeof (TimerCallback) == 0);
  assert (timer < 4 * TIMER_MAX_CALLBACKS);

  retval = timer_user[timer / 4];
  timer_user[timer / 4] = user_data;

  return retval;
}
