#ifndef MOUSE_H
#define MOUSE_H

#include <SDL.h>

void mouse_initialise (void);
void mouse_handle_event (const SDL_Event *ev);

#endif
