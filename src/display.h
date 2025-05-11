#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stddef.h>
#include "bfscreen.h"
#include "bfanywnd.h"

enum ColourLookUp {
    ColLU_BLACK     = 0,
    ColLU_WHITE,
    ColLU_RED,
    ColLU_GREEN,
    ColLU_BLUE,
    ColLU_YELLOW,
    ColLU_CYAN,
    ColLU_PINK,
    ColLU_GREYLT,//looks more like another white
    ColLU_GREYMD,
    ColLU_GREYDK,
    ColLU_BLINK_YLW_BLK,
    ColLU_BLINK_RED_WHT,
};

struct ScreenBufBkp {
    ubyte *WScreen;
    long PhysicalScreenWidth;
    long PhysicalScreenHeight;
    long GraphicsScreenWidth;
    long GraphicsScreenHeight;
    struct TbAnyWindow GWindow;
};

extern TbScreenMode screen_mode_game_hi;
extern TbScreenMode screen_mode_game_lo;
extern TbScreenMode screen_mode_menu;
extern TbScreenMode screen_mode_fmvid_lo;
extern TbScreenMode screen_mode_fmvid_hi;

extern ubyte game_high_resolution;

extern ubyte *display_palette;

extern ubyte *back_buffer;
extern TbPixel fade_unaffected_colours[];
extern TbPixel colour_lookup[];
extern ulong unkn_changing_color_counter1;
extern long dword_1AA270;

extern TbPixel colour_mix_lookup[];
extern TbPixel colour_sel_grey[];
extern TbPixel colour_sel_green[];
extern TbPixel colour_sel_blue[];
extern TbPixel colour_sel_red[];
extern TbPixel colour_sel_purple[];

extern TbPixel colour_grey1;
extern TbPixel colour_grey2;
extern TbPixel colour_brown2;

extern long engn_xc;
extern long engn_yc;
extern long engn_zc;
extern long engn_anglexz;

extern ushort text_window_x1;
extern ushort text_window_y1;
extern ushort text_window_x2;
extern ushort text_window_y2;

// TODO move engine texture atlas to separate file
extern ubyte *vec_tmap[18];

void display_set_full_screen(bool full_screen);
void display_set_lowres_stretch(bool stretch);
void display_lock(void);
void display_unlock(void);

void setup_screen_mode(TbScreenMode mode);
void setup_simple_screen_mode(TbScreenMode mode);

void screen_save_backup_buffer(struct ScreenBufBkp *bkp);
void screen_switch_to_custom_buffer(struct ScreenBufBkp *bkp,
  TbPixel *buf, short width, short height);
void screen_load_backup_buffer(struct ScreenBufBkp *bkp);

void update_unkn_changing_colors(void);
void setup_color_lookups(void);

const char * SWResourceMapping(short index);
void swap_wscreen(void);
void show_black_screen(void);
TbResult cover_screen_rect_with_sprite(short x, short y,
  ushort w, ushort h, struct TbSprite *spr);
TbResult cover_screen_rect_with_raw_file(short x, short y,
  ushort w, ushort h, const char *fname);


void my_set_text_window(ushort x1, ushort y1, ushort w, ushort h);
u32 my_string_width(const char *text);
u32 my_str_len(const char *t);
ubyte font_height(uchar c);

void change_brightness(short val);

#endif
