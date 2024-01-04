#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <stdbool.h>
#include <stddef.h>
#include "bfscreen.h"

extern TbScreenMode screen_mode_game_hi;
extern TbScreenMode screen_mode_game_lo;
extern TbScreenMode screen_mode_menu;
extern TbScreenMode screen_mode_fmvid_lo;
extern TbScreenMode screen_mode_fmvid_hi;

extern ubyte game_high_resolution;

extern ubyte *back_buffer;
extern struct TbSprite *unk3_sprites;
extern TbPixel fade_unaffected_colours[];
extern TbPixel colour_lookup[];

extern long engn_xc;
extern long engn_yc;
extern long engn_zc;

extern struct TbSprite *pointer_sprites;
extern struct TbSprite *pointer_sprites_end;
extern struct TbSprite *small_font;
extern struct TbSprite *small_font_end;
extern struct TbSprite *small2_font;
extern struct TbSprite *small2_font_end;
extern struct TbSprite *small_med_font;
extern struct TbSprite *small_med_font_end;
extern struct TbSprite *med_font;
extern struct TbSprite *med_font_end;
extern struct TbSprite *med2_font;
extern struct TbSprite *med2_font_end;
extern struct TbSprite *big_font;
extern struct TbSprite *big_font_end;

void display_set_full_screen(bool full_screen);
void display_set_lowres_stretch(bool stretch);
void display_lock(void);
void display_unlock(void);

void setup_screen_mode(TbScreenMode mode);
void setup_simple_screen_mode(TbScreenMode mode);

const char * SWResourceMapping(short index);
void swap_wscreen(void);
void screen_buffer_fill_black(void);
void show_black_screen(void);
void my_set_text_window(ushort x1, ushort y1, ushort w, ushort h);
ulong my_string_width(const char *text);
ubyte font_height(uchar c);

#endif
