#ifndef MOUSE_H
#define MOUSE_H

#include <SDL.h>
#include "bftypes.h"

struct TbPoint;

void mouse_initialise (void);
TbResult MEvent(const SDL_Event *ev);

#endif
