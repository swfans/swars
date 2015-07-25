#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <stdbool.h>
#include <stddef.h>

void display_initialise (void);
void display_finalise (void);
void display_update (void);
void display_set_full_screen (bool full_screen);
void display_set_lowres_stretch (bool stretch);
bool display_is_stretching_enabled (void);
void display_get_physical_size (size_t *width, size_t *height);
void display_get_size (size_t *width, size_t *height);
void *display_get_buffer (void);
void display_get_palette (SDL_Color *colours);
void display_update_mouse_pointer (void);
void display_lock (void);
void display_unlock (void);

#endif
