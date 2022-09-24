#include <SDL.h>
#include <assert.h>
#include <stdint.h>

#include "timer.h"

#include "sound.h"

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
