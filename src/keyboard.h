#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL.h>
#include "bftypes.h"

void keyboard_handle_event(const SDL_Event *ev);
ulong next_buffered_key(void);

#endif
