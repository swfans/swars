#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "display.h"

#include "bffile.h"
#include "bfscreen.h"
#include "bfscrsurf.h"
#include "bfpalette.h"
#include "bfsprite.h"
#include "bffont.h"
#include "bftext.h"
#include "bfmouse.h"
#include "bfplanar.h"
#include "bfutility.h"
#include "poly.h"

#include "game_sprts.h"
#include "util.h"
#include "swlog.h"

TbScreenMode screen_mode_game_hi = Lb_SCREEN_MODE_640_480_8;
TbScreenMode screen_mode_game_lo = Lb_SCREEN_MODE_320_200_8;
TbScreenMode screen_mode_menu = Lb_SCREEN_MODE_640_480_8;
TbScreenMode screen_mode_fmvid_lo = Lb_SCREEN_MODE_320_200_8;
TbScreenMode screen_mode_fmvid_hi = Lb_SCREEN_MODE_640_480_8;

extern ushort data_1aa330;
extern ushort data_1aa332;

TbPixel fade_unaffected_colours[] = {
  1,2,3,
  128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
  224,225,226,227,228,229,230,231,
  240,241,242,243,244,245,246,
  254,255,
  0};

#if defined(WIN32)

const char * SWResourceMapping(short index)
{
    switch (index)
    {
    case 1:
        return "A";
        //return MAKEINTRESOURCE(110); -- may work for other resource compilers
    default:
        return NULL;
    }
}

#else

const char * SWResourceMapping(short index)
{
    switch (index)
    {
    case 1:
        return "swars_icon.png";
    default:
        return NULL;
    }
}

#endif

void swap_wscreen(void)
{
    TbBool was_locked;
    was_locked = LbScreenIsLocked();
    if ( was_locked )
        LbScreenUnlock();
    LbScreenSwap();
    if ( was_locked )
    {
      while ( LbScreenLock() != Lb_SUCCESS )
        ;
    }
}

void
display_set_full_screen (bool full_screen)
{
    int i;

    if (lbScreenSurface != NULL)
        return;

    for (i = 1; i < LB_MAX_SCREEN_MODES_COUNT; ++i)
    {
        TbScreenModeInfo *mdinfo;

        mdinfo = LbScreenGetModeInfo(i);
        if (mdinfo->Width == 0) break;
        if (full_screen) {
            mdinfo->VideoMode &= ~Lb_VF_WINDOWED;
        } else {
            mdinfo->VideoMode |= Lb_VF_WINDOWED;
        }
    }
}

void display_set_lowres_stretch(bool stretch)
{
    if (stretch)
        LbScreenSetMinScreenSurfaceDimension(400);
    else
        LbScreenSetMinScreenSurfaceDimension(1);
}

void display_lock(void)
{
    if (!LbScreenIsLocked()) {
        while (LbScreenLock() != Lb_SUCCESS)
            ;
    }
}

void display_unlock(void)
{
    LbScreenUnlock();
}

void setup_simple_screen_mode(TbScreenMode mode)
{
    TbScreenModeInfo *mdinfo;
    short ratio;

    printf("%s %d\n", __func__, (int)mode);
    mdinfo = LbScreenGetModeInfo(mode);
    if (mdinfo->Width == 0) {
        LOGERR("Simple video mode %d is invalid", (int)mode);
        return;
    }
    LbScreenSetup(mode, mdinfo->Width, mdinfo->Height, display_palette);

    if (lbDisplay.GraphicsScreenHeight < 400)
        ratio = 2;
    else
        ratio = 1;
    LbMouseSetup(NULL, ratio, ratio);
}

void setup_screen_mode(TbScreenMode mode)
{
    TbBool was_locked;
    TbScreenModeInfo *mdinfo;
    short ratio;

    printf("%s %d\n", __func__, (int)mode);
    mdinfo = LbScreenGetModeInfo(mode);
    if (mdinfo->Width == 0) {
        LOGERR("Game video mode %d is invalid", (int)mode);
        mode = 1;
        mdinfo = LbScreenGetModeInfo(mode);
    }
    was_locked = LbScreenIsLocked();
    if (was_locked)
        LbScreenUnlock();
    if (LbScreenSetup(mode, mdinfo->Width, mdinfo->Height, display_palette) != 1)
        exit(1);
    if (was_locked) {
        while (LbScreenLock() != Lb_SUCCESS)
            ;
    }

    if (lbDisplay.GraphicsScreenHeight < 400)
        ratio = 2;
    else
        ratio = 1;
    LbMouseSetup(&pointer_sprites[1], ratio, ratio);

    setup_vecs(lbDisplay.WScreen, vec_tmap[0], lbDisplay.PhysicalScreenWidth,
        lbDisplay.PhysicalScreenWidth, lbDisplay.PhysicalScreenHeight);
}

void screen_save_backup_buffer(struct ScreenBufBkp *bkp)
{
    LbScreenStoreGraphicsWindow(&bkp->GWindow);
    bkp->WScreen = lbDisplay.WScreen;
    bkp->PhysicalScreenWidth = lbDisplay.GraphicsScreenWidth;
    bkp->PhysicalScreenHeight = lbDisplay.PhysicalScreenHeight;
    bkp->GraphicsScreenWidth = lbDisplay.GraphicsScreenWidth;
    bkp->GraphicsScreenHeight = lbDisplay.GraphicsScreenHeight;
}

void screen_switch_to_custom_buffer(struct ScreenBufBkp *bkp,
  TbPixel *buf, short width, short height)
{
    screen_save_backup_buffer(bkp);
    lbDisplay.WScreen = buf;
    lbDisplay.GraphicsScreenWidth = width;
    lbDisplay.PhysicalScreenHeight = height;
    lbDisplay.GraphicsScreenWidth = width;
    lbDisplay.GraphicsScreenHeight = height;
    LbScreenSetGraphicsWindow(0, 0, width, height);
}

void screen_load_backup_buffer(struct ScreenBufBkp *bkp)
{
    lbDisplay.WScreen = bkp->WScreen;
    lbDisplay.GraphicsScreenWidth = bkp->PhysicalScreenWidth;
    lbDisplay.PhysicalScreenHeight = bkp->PhysicalScreenHeight;
    lbDisplay.GraphicsScreenWidth = bkp->GraphicsScreenWidth;
    lbDisplay.GraphicsScreenHeight = bkp->GraphicsScreenHeight;
    LbScreenLoadGraphicsWindow(&bkp->GWindow);
}

void show_black_screen(void)
{
    TbBool was_locked;

    was_locked = LbScreenIsLocked();
    if (!was_locked) {
        while (LbScreenLock() != Lb_SUCCESS)
            ;
    }
    LbScreenClear(0);
    if (!was_locked)
        LbScreenUnlock();
    swap_wscreen();
}

TbResult cover_screen_rect_with_sprite(short x, short y, ushort w, ushort h, struct TbSprite *spr)
{
    short cx, cy;
    TbResult ret;

    ret = Lb_FAIL;
    for (cy = y; cy < y+h; cy += spr->SHeight)
    {
        for (cx = x; cx < x+w; cx += spr->SWidth) {
            ret = LbSpriteDraw(cx, cy, spr);
        }
    }
    return ret;
}

TbResult cover_screen_rect_with_raw_file(short x, short y, ushort w, ushort h, const char *fname)
{
    struct SSurface surf;
    struct TbRect srect;
    ubyte *inp_buf;
    TbResult ret;

    LbSetRect(&srect, 0, 0, w, h);
    LbScreenSurfaceInit(&surf);
    LbScreenSurfaceCreate(&surf, w, h);
    inp_buf = LbScreenSurfaceLock(&surf);
    ret = LbFileLoadAt(fname, inp_buf);
    LbScreenSurfaceUnlock(&surf);
    if (ret == Lb_FAIL) {
        LbScreenSurfaceRelease(&surf);
        return ret;
    }
    LbScreenUnlock();
    ret = LbScreenSurfaceBlit(&surf, x, y, &srect, SSBlt_FLAG8 | SSBlt_FLAG4);
    LbScreenSurfaceRelease(&surf);
    LbScreenLock();
    return ret;
}

void update_unkn_changing_colors(void)
{
    ubyte col1, col2;

    unkn_changing_color_counter1++;

    if (unkn_changing_color_counter1 & 0x01)
        col1 = colour_lookup[ColLU_YELLOW];
    else
        col1 = colour_lookup[ColLU_BLACK];
    colour_lookup[ColLU_BLINK_YLW_BLK] = col1;

    if (unkn_changing_color_counter1 & 0x01)
        col2 = colour_lookup[ColLU_RED];
    else
        col2 = colour_lookup[ColLU_WHITE];
    colour_lookup[ColLU_BLINK_RED_WHT] = col2;
}

void setup_color_lookups(void)
{
#if 0
    asm volatile ("call ASM_setup_color_lookups\n"
        :  :  : "eax" );
#endif
    dword_1AA270 = 1;
    colour_lookup[ColLU_BLACK] = LbPaletteFindColour(display_palette, 0, 0, 0);
    colour_lookup[ColLU_WHITE] = LbPaletteFindColour(display_palette, 63, 63, 63);
    colour_lookup[ColLU_RED] = LbPaletteFindColour(display_palette, 63, 0, 0);
    colour_lookup[ColLU_GREEN] = LbPaletteFindColour(display_palette, 0, 63, 0);
    colour_lookup[ColLU_BLUE] = LbPaletteFindColour(display_palette, 0, 0, 63);
    colour_lookup[ColLU_YELLOW] = LbPaletteFindColour(display_palette, 63, 63, 0);
    colour_lookup[ColLU_CYAN] = LbPaletteFindColour(display_palette, 0, 63, 63);
    colour_lookup[ColLU_PINK] = LbPaletteFindColour(display_palette, 63, 0, 63);
    colour_lookup[ColLU_GREYLT] = LbPaletteFindColour(display_palette, 50, 50, 50);
    colour_lookup[ColLU_GREYMD] = LbPaletteFindColour(display_palette, 30, 30, 30);
    colour_lookup[ColLU_GREYDK] = LbPaletteFindColour(display_palette, 10, 10, 10);
    dword_1AA270 = 0;
    update_unkn_changing_colors();

    colour_mix_lookup[0] = LbPaletteFindColour(display_palette, 0, 63, 0);
    colour_mix_lookup[1] = LbPaletteFindColour(display_palette, 38, 48, 63);
    colour_mix_lookup[2] = LbPaletteFindColour(display_palette, 0, 63, 63);
    colour_mix_lookup[3] = LbPaletteFindColour(display_palette, 63, 63, 0);
    colour_mix_lookup[4] = LbPaletteFindColour(display_palette, 63, 0, 63);
    colour_mix_lookup[5] = LbPaletteFindColour(display_palette, 63, 32, 32);
    colour_mix_lookup[6] = LbPaletteFindColour(display_palette, 32, 63, 32);
    colour_mix_lookup[7] = LbPaletteFindColour(display_palette, 32, 32, 63);
    colour_mix_lookup[8] = LbPaletteFindColour(display_palette, 32, 32, 32);
    colour_mix_lookup[9] = LbPaletteFindColour(display_palette, 32, 63, 63);
    colour_mix_lookup[10] = LbPaletteFindColour(display_palette, 63, 63, 32);
    colour_mix_lookup[11] = LbPaletteFindColour(display_palette, 63, 32, 63);

    colour_sel_grey[0] = LbPaletteFindColour(display_palette, 48, 48, 48);
    colour_sel_grey[1] = LbPaletteFindColour(display_palette, 40, 40, 40);
    colour_sel_grey[2] = LbPaletteFindColour(display_palette, 32, 32, 32);
    colour_sel_grey[3] = LbPaletteFindColour(display_palette, 24, 24, 24);

    colour_sel_green[0] = LbPaletteFindColour(display_palette, 0, 48, 0);
    colour_sel_green[1] = LbPaletteFindColour(display_palette, 0, 40, 0);
    colour_sel_green[2] = LbPaletteFindColour(display_palette, 0, 32, 0);
    colour_sel_green[3] = LbPaletteFindColour(display_palette, 0, 24, 0);

    colour_sel_blue[0] = LbPaletteFindColour(display_palette, 0, 0, 48);
    colour_sel_blue[1] = LbPaletteFindColour(display_palette, 0, 0, 40);
    colour_sel_blue[2] = LbPaletteFindColour(display_palette, 0, 0, 32);
    colour_sel_blue[3] = LbPaletteFindColour(display_palette, 0, 0, 24);

    colour_sel_red[0] = LbPaletteFindColour(display_palette, 48, 0, 0);
    colour_sel_red[1] = LbPaletteFindColour(display_palette, 40, 0, 0);
    colour_sel_red[2] = LbPaletteFindColour(display_palette, 32, 0, 0);
    colour_sel_red[3] = LbPaletteFindColour(display_palette, 24, 0, 0);

    colour_sel_purple[0] = LbPaletteFindColour(display_palette, 48, 0, 48);
    colour_sel_purple[1] = LbPaletteFindColour(display_palette, 40, 0, 40);
    colour_sel_purple[2] = LbPaletteFindColour(display_palette, 32, 0, 32);
    colour_sel_purple[3] = LbPaletteFindColour(display_palette, 24, 0, 24);
}

void my_set_text_window(ushort x1, ushort y1, ushort w, ushort h)
{
#if 0
    asm volatile (
      "call ASM_my_set_text_window\n"
        : : "a" (x1), "d" (y1), "b" (w), "c" (h));
#endif
    text_window_x1 = x1;
    text_window_y1 = y1;
    text_window_x2 = x1 + w - 1;
    text_window_y2 = y1 + h - 1;
}

ubyte font_height(uchar c)
{
#if 0
    int ret;
    asm volatile ("call ASM_font_height\n"
        : "=r" (ret) : "a" (c));
    return ret;
#endif
    if (lbFontPtr == small_font || lbFontPtr == small2_font)
    {
        return LbSprFontCharHeight(lbFontPtr, c) - 1;
    }
    else if (lbFontPtr == small_med_font)
    {
        if (c < 97 || c > 122)
        {
          return LbSprFontCharHeight(lbFontPtr, c) - 2;
        }
        else
        {
          return LbSprFontCharHeight(lbFontPtr, c);
        }
    }
    else if (lbFontPtr == med_font || lbFontPtr == med2_font)
    {
        return LbSprFontCharHeight(lbFontPtr, c) - 2;
    }
    else if (lbFontPtr == big_font)
    {
         return LbSprFontCharHeight(lbFontPtr, c) - 4;
    }
    else
    {
        return LbSprFontCharHeight(lbFontPtr, c);
    }
}

u32 my_string_width(const char *text)
{
    int ret;
    asm volatile ("call ASM_my_string_width\n"
        : "=r" (ret) : "a" (text));
    return ret;
}

u32 my_str_len(const char *t)
{
    return strlen(t);
}

void change_brightness(short val)
{
    asm volatile ("call ASM_change_brightness\n"
        : : "a" (val));
}
