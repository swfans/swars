/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file devcreds.c
 *     Developer team credits list display.
 * @par Purpose:
 *     Functions for printing a list of game developers, testers and artists.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     17 Sep 2023 - 17 Oct 2026
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "devcreds.h"

#include <stdlib.h>
#include <assert.h>

#include "bffile.h"
#include "bffont.h"
#include "bfgentab.h"
#include "bfmath.h"
#include "bfscreen.h"
#include "bfsprite.h"
#include "bftext.h"
#include "bfutility.h"

#include "engincam.h"
#include "enginpeff.h"

#include "bfscd.h"
#include "ssampply.h"

#include "display.h"
#include "fmvplay.h"
#include "game_data.h"
#include "game_speed.h"
#include "game_sprts.h"
#include "game.h"
#include "keyboard.h"
#include "lvdraw3d.h"
#include "mydraw.h"
#include "sound.h"
#include "swlog.h"
#include "thing.h"
#include "vehtraffic.h"
#include "wadfile.h"
/******************************************************************************/

/** Expected sizes for font DAT/TAB files for resolution 320x200.
 * Each file has 205 sprites, TAB has 6 bytes per entry, DAT varies to use empirical value.
 */
#define EXPECTED_FONT_DAT_TAB_SIZE_8 (205 * 6 + 205 * 34)

/** Expected sizes for translation strings TXT files.
 */
#define EXPECTED_LANG_TXT_SIZE 8000

s32 devteam_credits_last_played_smp = -1;
char *devteam_credits_desc[100];
char *devteam_credits_groups[40];
s32 devteam_credits_group_no;
s32 devteam_credits_groups_count;
TbBool devteam_credits_enable = false;

const char *outro_story_text;
const char *outro_header_text;

u32 colwavapr_timer;
u32 colwavapr_reveal_tm;
TbBool colwav_enable = false;

/******************************************************************************/

int load_devteam_credits_text(ubyte *buf)
{
    int totlen;
    int peptxt_len;
    int groupno;
    char *name;
    char *desc;
    char *s;
    int i;

    totlen = LbFileLoadAt("data/people.txt", buf);
    if (totlen == -1) {
        buf[0] = '\0';
        return 0;
    }
    peptxt_len = totlen;

    s = (char *)buf;
    for (i = 0; i < peptxt_len; i++)
    {
        if (*s == ']')
            *s = '\0';
        s++;
    }

    // List of people
    groupno = 0;
    s = (char *)buf;
    while ( 1 )
    {
        if (*s == '[')
        {
            s++;
            name = s;
            if (strcmp(name, "END") == 0) {
              s += 4;
              break;
            }
            devteam_credits_desc[2 * groupno + 0] = name;

            while (*s != '[')
            {
                if (*s == '#') {
                    groupno = strtol(s + 1, &desc, 10);
                    s = desc;
                }
                s++;
            }
            s++;
            desc = s;
            devteam_credits_desc[2 * groupno + 1] = desc;
            continue;
        }
        if (*s == '#') {
            groupno = strtol(s + 1, &desc, 10);
            s = desc;
        }
        s++;
    }

    // Assignment of people to groups
    while ( 1 )
    {
        if (*s == '[')
        {
            ubyte *g;

            s++;
            if (strcmp(s, "END") == 0) {
                s += 4;
                break;
            }
            devteam_credits_groups[2 * groupno + 0] = s;

            while (*s != '[')
            {
                if (*s == '#') {
                    groupno = strtol(s + 1, &desc, 10);
                    s = desc;
                }
                s++;
            }
            s++;
            g = &buf[totlen];
            devteam_credits_groups[2 * groupno + 1] = (char *)g; // TODO we should really use a struct here

            // Recognize the list of integers, store them in g
            while ( 1 )
            {
              *g = strtol(s, &desc, 10);
              if (desc > s) {
                  totlen++;
                  s = desc;
                  g++;
              }
              if (*s == '\0')
                  break;
              s++;
            }
            *g = 100; // end-of-list marker
            totlen++;
            continue;
        }
        if (*s == '#')
        {
            groupno = strtol(s + 1, &desc, 10);
            s = desc;
        }
        s++;
    }
    devteam_credits_groups_count = groupno + 1;

    return totlen;
}

/** Get fade level to be used while drawing next character of a text.
 *
 *  Computes the colour-remap fade level to be used while drawing the next
 *  character of a text, advancing the shared letter-reveal counter
 *  (cwa_revl_tm) towards the current progress (cwa_timer).
 *
 * @param fade Receives the computed fade level (0..63 range index offset) if
 *     the character should be drawn. Write-only value.
 * @return True if the character is already revealed and should be drawn.
 */
static TbBool credits_next_char_fade(s32 *fade, u32 *p_cwa_revl_tm,
  u32 cwa_timer, u32 cw_shift)
{
    s32 f;

    if (*p_cwa_revl_tm > cwa_timer)
        return false;
    (*p_cwa_revl_tm)++;
    f = (cwa_timer - *p_cwa_revl_tm) * 2;
    if (f > 0x1F)
        f = 0x1F;
    if (colwav_enable)
        f += lbSinTable[((*p_cwa_revl_tm + cw_shift) & 0xF) * 128] >> 13;
    if (f < 0)
        f = 0;
    *fade = f;
    return true;
}

/** @internal
 * Puts colour wave text sprites with appearing animation on screen.
 * @param sbuf
 * @param ebuf
 * @param x
 * @param y
 * @param space_len
 */
void put_down_colwavaprtext_ini_sprites(const char *sbuf, const char *ebuf,
  s32 scr_x, s32 scr_y, s32 space_len,
  u32 *p_cwa_revl_tm, u32 cwa_timer, u32 cw_shift)
{
    const char *c;
    int x, y;
    ubyte chr;

    x = scr_x;
    y = scr_y;
    for (c=sbuf; c < ebuf; c++)
    {
        const TbSprite *p_spr;
        s32 fade;

        chr = (ubyte)(*c);

        if (chr == '\n')
        {
            y += LbTextLineHeight() * 5 / 6;
            x = scr_x;
            continue;
        }
        else if (chr == ' ')
        {
            ubyte *p_scan;
            s32 word_w;
            ubyte cs;

            word_w = 0;
            p_scan = (ubyte *)c + 1;
            if (*p_scan != chr)
            {
                while (1)
                {
                    cs = *p_scan;
                    if ((cs == 0) || (cs == '\n') || (cs == '['))
                        break;
                    word_w += LbTextCharWidth(cs);
                    if (*(p_scan + 1) != ' ') {
                        p_scan++;
                    } else {
                        break;
                    }
                }
            }
            if (x + word_w <= lbDisplay.PhysicalScreenWidth - 6) {
                x += space_len;
            } else {
                y += LbTextLineHeight() * 5 / 6;
                x = scr_x;
            }
            continue;
        }

        p_spr = LbFontCharSprite(lbFontPtr, chr);
        if (p_spr == NULL)
            continue;

        if (credits_next_char_fade(&fade, p_cwa_revl_tm, cwa_timer, cw_shift)) {
            const TbPixel *pxmap;

            pxmap = &pixmap.fade_table[256 * (0x3F - fade)];
            LbSpriteDrawRemap(x, y, p_spr, pxmap);
        }
        x += p_spr->SWidth;
    }
}

int height_colwavaprtext_ini_sprites(const char *sbuf, const char *ebuf,
  s32 scr_x, s32 scr_y, short space_len)
{
    const char *c;
    int x, y;
    ubyte chr;

    x = scr_x;
    y = scr_y;
    for (c=sbuf; c < ebuf; c++)
    {
        const TbSprite *p_spr;

        chr = (ubyte)(*c);

        if (chr == '\n')
        {
            y += LbTextLineHeight() * 5 / 6;
            x = scr_x;
            continue;
        }
        else if (chr == ' ')
        {
            ubyte *p_scan;
            s32 word_w;
            ubyte cs;

            word_w = 0;
            p_scan = (ubyte *)c + 1;
            if (*p_scan != chr)
            {
                while (1)
                {
                    cs = *p_scan;
                    if ((cs == 0) || (cs == '\n') || (cs == '['))
                        break;
                    word_w += LbTextCharWidth(cs);
                    if (*(p_scan + 1) != ' ') {
                        p_scan++;
                    } else {
                        break;
                    }
                }
            }
            if (x + word_w <= lbDisplay.PhysicalScreenWidth - 6) {
                x += space_len;
            } else {
                y += LbTextLineHeight() * 5 / 6;
                x = scr_x;
            }
            continue;
        }

        p_spr = LbFontCharSprite(lbFontPtr, chr);
        if (p_spr == NULL)
            continue;

        x += p_spr->SWidth;
    }
    if (x != scr_x)
        y += LbTextLineHeight() * 5 / 6;
    return y - scr_y;
}

void draw_colwavapr_text_paragraph(int scr_x, int *p_scr_y,
  const struct TbSprite *p_font, const char *text)
{
    const char *s;
    const char *e;
    int x, y;
    short space_len;

    x = scr_x;
    y = *p_scr_y;

    s = text;
    e = strchr(s, '[');
    if (e == NULL)
        e = s + strlen(s);

    lbFontPtr = p_font;
    space_len = LbTextCharWidth(' ') / 2;

    put_down_colwavaprtext_ini_sprites(s, e, x, y, space_len,
      &colwavapr_reveal_tm, colwavapr_timer, gameturn);

    y += height_colwavaprtext_ini_sprites(s, e, x, y, space_len);
    *p_scr_y = y;
}

void func_cc0d4(char **text_pargr)
{
    ubyte *p_ids;
    int x, y;
    int par_padding;

    colwav_enable = true;
    colwavapr_reveal_tm = 0;

    lbFontPtr = med2_font;
    par_padding = LbTextLineHeight() / 3;

    // Draw the credits group header (list of roles belonging to this group)
    x = 6;
    y = par_padding;
    draw_colwavapr_text_paragraph(x, &y, med_font, text_pargr[0]);
    // Draw every person assigned to this group (name, then description)
    p_ids = (ubyte *)text_pargr[1];
    x = 12;
    while (*p_ids != 100)
    {
        char **p_person;

        p_person = &devteam_credits_desc[2 * (*p_ids)];
        colwav_enable = true;

        if (colwavapr_reveal_tm == colwavapr_timer)
        {
            short smp;

            if (devteam_credits_last_played_smp != -1)
                stop_sample_using_heap(0, devteam_credits_last_played_smp);
            smp = 2 + (LbRandomAnyShort() % 5);
            devteam_credits_last_played_smp = smp;
            play_sample_using_heap(0, smp, FULL_VOL, EQUL_PAN, NORM_PTCH, LOOP_NO, 3u);
        }

        // Header is the person's name, paragraphed text is person's description
        draw_colwavapr_text_paragraph(x, &y, big_font, p_person[0]);
        draw_colwavapr_text_paragraph(x, &y, med2_font, p_person[1]);

        y += par_padding;
        p_ids++;
    }
}

void devteam_credits_show(void)
{
    devteam_credits_last_played_smp = -1;
    devteam_credits_enable = 0;
    devteam_credits_group_no = 0;
    colwavapr_timer = 0;
    gameturn = 0;
    render_clock_set_turn(gameturn);

    play_sample_using_heap(0, 1, FULL_VOL, EQUL_PAN, NORM_PTCH, LOOP_4EVER, 3);

    data_1dd91c = 0;
    overall_scale = 40;
    set_brightness_fadedout();

    while (1)
    {
        if (is_key_pressed(KC_SPACE, KMod_DONTCARE))
            break;
        if (is_key_pressed(KC_ESCAPE, KMod_DONTCARE))
            break;
        if (is_key_pressed(KC_RETURN, KMod_DONTCARE))
            break;

        if (gameturn < 128)
        {
            // 64 steps of brightness increase - fade in
            if (gameturn & 1)
                change_brightness(1);
            if (gameturn == 127)
                devteam_credits_enable = 1;
        } else {
            // Randomly play seagull sample if applause is not currently playing
            if (((LbRandomAnyShort() & 0xF) == 0) &&
              (devteam_credits_last_played_smp == -1 || !IsSamplePlaying(0, devteam_credits_last_played_smp, NULL)) )
            {
                play_sample_using_heap(0, 7 + (LbRandomAnyShort() % 5), FULL_VOL, EQUL_PAN, NORM_PTCH, LOOP_NO, 3u);
            }
        }

        gameturn++;
        render_clock_set_turn(gameturn);
        traffic_unkn_func_01();
        camera_apply_velocity();
        prepare_drawlist();
        process_sound_heap();
        engine_draw_whole_screen_flyby();
        if (devteam_credits_enable)
        {
            colwavapr_timer++;
            func_cc0d4((char **)&devteam_credits_groups[2 * devteam_credits_group_no]);
            if (colwavapr_timer > colwavapr_reveal_tm + 50)
            {
                colwavapr_timer = 0;
                devteam_credits_group_no++;
                if (devteam_credits_group_no == devteam_credits_groups_count)
                    devteam_credits_group_no = 0;
            }
          }
          swap_wscreen();
          game_update();
          LbScreenClear(0);
    }
    clear_key_pressed(KC_SPACE);
    clear_key_pressed(KC_ESCAPE);
    clear_key_pressed(KC_RETURN);

    StopAllSamples();
}

void screen_dark_curtain_down(void)
{
    int w;
    // Use 32-bit range to allow 8 lower bits to be fractions of a pixel
    u32 range_beg[MAX_SUPPORTED_SCREEN_WIDTH];
    u32 range_len[MAX_SUPPORTED_SCREEN_WIDTH];
    uint already_count;
    short angl_A, angl_B, angl_C;
    short dt_angl_A, dt_angl_B, dt_angl_C;

    dt_angl_B = 20 + (LbRandomAnyShort() & 0x1F);
    dt_angl_A = 20 + (LbRandomAnyShort() & 0x1F);
    dt_angl_C = 20 + (LbRandomAnyShort() & 0x1F);
    angl_B = LbRandomAnyShort() & LbFPMath_AngleMask;
    angl_A = LbRandomAnyShort() & LbFPMath_AngleMask;
    angl_C = LbRandomAnyShort() & LbFPMath_AngleMask;
    for (w = 0; w < 320; w++)
    {
        int mag;

        range_beg[w] = 0;
        mag = lbSinTable[(angl_A & LbFPMath_AngleMask) + 512];
        mag += lbSinTable[angl_B & LbFPMath_AngleMask];
        mag -= lbSinTable[angl_C & LbFPMath_AngleMask];
        range_len[w] = abs(mag) >> 6;

        angl_C += dt_angl_C;
        angl_A += dt_angl_A;
        angl_B += dt_angl_B;
    }

    do
    {
        int h_beg, h_end, h;

        already_count = 0;
        for (w = 0; w < 320; w++)
        {
            h_beg = range_beg[w] / (game_num_fps << 8);
            if (h_beg >= lbDisplay.GraphicsScreenHeight) {
                already_count++;
                continue;
            }
            h_end = (range_beg[w] + range_len[w]) / (game_num_fps << 8);
            for (h = h_beg; h < h_end; h++)
            {
                TbPixel px;
                uint scr_pos;

                if (h >= lbDisplay.GraphicsScreenHeight)
                    break;

                scr_pos = 320 * h + w;
                px = lbDisplay.WScreen[scr_pos];
                lbDisplay.WScreen[scr_pos] = pixmap.fade_table[px + 4096];
            }
            range_beg[w] += range_len[w];
            range_len[w] += (1 << 8) + (range_len[w] >> 5);
            if (range_len[w] > MAX_SUPPORTED_SCREEN_HEIGHT * (4 << 8))
                range_len[w] = MAX_SUPPORTED_SCREEN_HEIGHT * (4 << 8);
        }
        swap_wscreen();
        game_update();
    }
    while (already_count < 320);
}

void screen_outro_story_message_show(void)
{
    const char *text_hdr;
    const char *text_par;
    int x, y;

    text_hdr = outro_header_text;
    text_par = outro_story_text;

    colwavapr_timer = 0;
    gameturn = 0;

    while (1)
    {
        colwav_enable = false;
        colwavapr_reveal_tm = 0;

        x = 12;
        y = 14;

        draw_colwavapr_text_paragraph(x, &y, big_font, text_hdr);
        draw_colwavapr_text_paragraph(x, &y, med2_font, text_par);
        colwavapr_timer++;
        if (colwavapr_timer > colwavapr_reveal_tm + 20)
            break;

        swap_wscreen();
        wait_next_gameturn();
        gameturn++;
    }
}

int load_outro_text(ubyte *buf)
{
    int totlen;
    char *s;
    int i;

    outro_header_text = "";
    outro_story_text = "";

    totlen = load_file_alltext("outtro.txt", buf);
    if (totlen == -1) {
        buf[0] = '\0';
        return 0;
    }
    buf[totlen] = '\0';
    s = (char *)buf;
    // The font can handle lower case letters, upper case just looks better
    for (i = 0; i < totlen; i++) {
        s[i] = my_char_to_upper(s[i]);
    }

    // Primitive INI parsing - we expect 1st block to always be `header_win`
    s = (char *)buf;
    while ((*s != ']') && (*s != '\0')) {
        s++;
    }
    s++;
    outro_header_text = s;

    // Primitive INI parsing - we expect 2nd block to always be `message_win`
    while ((*s != ']') && (*s != '\0')) {
        s++;
    }
    s++;
    outro_story_text = s;

    return totlen + 1;
}

TbResult load_outro_sprites(void)
{
    PathInfo *pinfo;
    ubyte *p_buf;
    ubyte *outtxt_ptr;
    ubyte *peptxt_ptr;
    int next_pos, next_len;
    int total_allocs;
    TbResult tret, ret;
    short max_detail;

    max_detail = 0;//UNKN_sprites_scale / 2;
    tret = Lb_OK;

    pinfo = &game_dirs[DirPlace_Data];

    total_allocs = 3 * EXPECTED_FONT_DAT_TAB_SIZE_8 + 2 * EXPECTED_LANG_TXT_SIZE;
    next_pos = engine_mem_alloc_size - 64000 - total_allocs;

    assert(next_pos >= 0);
    p_buf = engine_mem_alloc_ptr + next_pos;

    ret = load_sprites_med_font(&p_buf, pinfo->directory, 7, max_detail);
    if (tret == Lb_OK)
        tret = ret;

    ret = load_sprites_big_font(&p_buf, pinfo->directory, 6, max_detail + 1);
    if (tret == Lb_OK)
        tret = ret;

    ret = load_sprites_med2_font(&p_buf, pinfo->directory, 12, max_detail);
    if (tret == Lb_OK)
        tret = ret;

    setup_sprites_med_font();
    setup_sprites_med2_font();
    setup_sprites_big_font();

    outtxt_ptr = p_buf;
    next_len = load_outro_text(outtxt_ptr);
    p_buf += next_len;

    peptxt_ptr = p_buf;
    next_len = load_devteam_credits_text(peptxt_ptr);
    p_buf += next_len;

    assert((p_buf - ((ubyte *)engine_mem_alloc_ptr + next_pos)) <= total_allocs);
    if (tret == Lb_FAIL) {
        LOGERR("Some files were not loaded successfully");
    }
    return tret;
}

void init_outro(void)
{
    gamep_scene_effect_type = ScEff_NONE;
    gamep_scene_effect_intensity = 1000;
    StopAllSamples();
    StopCD();

    play_smacker(MPly_Outro);

    screen_dark_curtain_down();

    init_things();
    //TODO hard-coded map ID
    change_current_map(51);
    load_outro_sprites();

    screen_outro_story_message_show();
    // Sleep for up to 10 seconds
    screen_wait_seconds_or_until_continue_key(10);

    LbPaletteFade(0, 200, 1);
    show_black_screen();
    StopAllSamples();
    reset_heaps();

    setup_heaps(SHSC_CreditsSnd, language_3str);
    devteam_credits_show();
    reset_heaps();

    setup_heaps(SHSC_GameSndBestQ, language_3str);
}

/******************************************************************************/
