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

#include "bffile.h"
#include "bffont.h"
#include "bfgentab.h"
#include "bfmath.h"
#include "bfscreen.h"
#include "bfsprite.h"
#include "bftext.h"
#include "bfutility.h"
#include <stdlib.h>

#include "engincam.h"

#include "ssampply.h"

#include "display.h"
#include "game_speed.h"
#include "game_sprts.h"
#include "game.h"
#include "keyboard.h"
#include "lvdraw3d.h"
#include "sound.h"
#include "swlog.h"
#include "vehtraffic.h"
/******************************************************************************/

s32 devteam_credits_last_played_smp = -1;
char *devteam_credits_desc[100];
char *devteam_credits_groups[40];
s32 devteam_credits_group_no;
s32 devteam_credits_groups_count;
TbBool devteam_credits_enable = false;

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
    //unkn_flags_01 = 1; //TODO seem unused - remove pending
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

/******************************************************************************/
