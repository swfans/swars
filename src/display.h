#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <stdbool.h>
#include <stddef.h>
#include "bfscreen.h"
#include "poly.h"

// TODO figure out why we have PolyPoint and EnginePoint
struct EnginePoint {
  struct PolyPoint pp;
  /*long X;
  long Y;
  long TMapX;
  long TMapY;
  long Shade;*/
  long X3d;
  long Y3d;
  long Z3d;
  long DistSqr;
  ulong Flags; // TODO check if it is ushort
};


extern TbScreenMode screen_mode_game_hi;
extern TbScreenMode screen_mode_game_lo;
extern TbScreenMode screen_mode_menu;
extern TbScreenMode screen_mode_fmvid;

extern ubyte *back_buffer;
extern struct TbSprite *unk3_sprites;
extern TbPixel fade_unaffected_colours[];
extern TbPixel colour_lookup[];

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

void display_set_full_screen (bool full_screen);
void display_set_lowres_stretch (bool stretch);
void display_lock (void);
void display_unlock (void);
void setup_screen_mode(TbScreenMode mode);
const char * SWResourceMapping(short index);
void swap_wscreen(void);
void screen_buffer_fill_black(void);
void show_black_screen(void);
void my_set_text_window(ushort x1, ushort y1, ushort w, ushort h);
ulong my_string_width(const char *text);
ubyte font_height(uchar c);

void draw_trigpoly(struct PolyPoint *point_a, struct PolyPoint *point_b, struct PolyPoint *point_c);

#endif
