#include <SDL.h>
#include <assert.h>
#include <stdint.h>

#include "timer.h"

#include "sound.h"

extern TimerCallback timer_callbacks[TIMER_MAX_CALLBACKS];
extern uint32_t	     timer_callback_states[TIMER_MAX_CALLBACKS];
extern void         *timer_callback_arguments[TIMER_MAX_CALLBACKS];
extern uint32_t	     timer_divisor;

void
timer_set_divisor (uint32_t divisor)
{
  timer_divisor = divisor;
}

uint32_t
timer_get_18_2_hz_ticks (void)
{
  return (((uint64_t) SDL_GetTicks () * 1193182 / 1000) >> 15);
}

uint32_t
timer_get_1000hz_ticks (void)
{
  return SDL_GetTicks ();
}

TimerId
timer_register_callback (TimerCallback fn)
{
  size_t n;

  for (n = 0; n < TIMER_MAX_CALLBACKS - 1; n++)
    {
      if (timer_callback_states[n] != 0)
	continue;

      timer_callback_states[n] = 1;
      timer_callbacks[n]       = fn;

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

  retval = timer_callback_arguments[timer / 4];
  timer_callback_arguments[timer / 4] = user_data;

  return retval;
}
