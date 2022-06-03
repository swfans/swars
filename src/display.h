#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <stdbool.h>
#include <stddef.h>
#include "bfscreen.h"

extern ubyte *back_buffer;

void display_set_full_screen (bool full_screen);
void display_set_lowres_stretch (bool stretch);
void display_lock (void);
void display_unlock (void);
void setup_screen_mode(TbScreenMode mode);
const char * SWResourceMapping(short index);
void swap_wscreen(void);
void show_black_screen(void);
void setup_vecs(ubyte *screenbuf, ubyte *vec_tmap, ulong width3, ulong width4, long height);
void my_set_text_window(ushort x1, ushort y1, ushort w, ushort h);

#endif
