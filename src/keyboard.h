#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL.h>

void keyboard_initialise (void);
void keyboard_handle_event (const SDL_Event *ev);

#endif
