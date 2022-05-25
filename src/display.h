#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <stdbool.h>
#include <stddef.h>

void display_update (void);
void display_set_full_screen (bool full_screen);
void display_set_lowres_stretch (bool stretch);
void display_lock (void);
void display_unlock (void);
const char * SWResourceMapping(short index);
void swap_wscreen(void);

#endif
