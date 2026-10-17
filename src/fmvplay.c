/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file fmvplay.c
 *     Wrapper of Smack player which help with Full Motion Video playback.
 * @par Purpose:
 *     Implements easy to use functions which handle FMV files playback.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     13 Apr 2023 - 16 Oct 2026
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "fmvplay.h"

#include <assert.h>
#include "bfmouse.h"
#include "bfpalette.h"

#include "ssampply.h"

#include "engincolour.h"

#include "bfsmack.h"

#include "campaign.h"
#include "display.h"
#include "game_data.h"
#include "game_options.h"
#include "game.h"
#include "keyboard.h"
#include "sound.h"
#include "swlog.h"
/******************************************************************************/

u32 smack_malloc_used_tot = 0;

/******************************************************************************/

void sprint_fmv_filename(ushort vid_type, char *fnbuf, ulong buflen)
{
    const char *fname;
    PathInfo *pinfo;
    struct Campaign *p_campgn;

    fname = NULL;
    switch (vid_type)
    {
    case MPly_Intro:
        // Intro name is hard-coded, as it is played before any campaign is loaded
        fname = "intro.smk";
        pinfo = &game_dirs[DirPlace_LangData];
        if (game_dirs[DirPlace_Sound].use_cd == 1)
            sprintf(fnbuf, "%s%s/%s", cd_drive, pinfo->directory, fname);
        else
            sprintf(fnbuf, "intro/%s", fname);
        break;
    case MPly_MissiComplete:
        switch (background_type)
        {
        case 0:
            fname = "syn_mc.smk";
            sprintf(fnbuf, "qdata/%s", fname);
            break;
        case 1:
            fname = "zel-mc.smk";
            sprintf(fnbuf, "qdata/%s", fname);
            break;
        default:
            fnbuf[0] = '\0';
            break;
        }
        break;
    case MPly_MPartComplete:
        switch (background_type)
        {
        case 0:
            // TODO MAPNO case for a specific map, remove
            if (current_map == 46)
            {
                fname = "syn_ele.smk";
                pinfo = &game_dirs[DirPlace_LangData];
                if (game_dirs[DirPlace_Data].use_cd == 1)
                    sprintf(fnbuf, "%s%s/%s", cd_drive, pinfo->directory, fname);
                else
                    sprintf(fnbuf, "data/%s", fname);
            }
            else
            {
                fnbuf[0] = '\0';
            }
            break;
        case 1:
            // TODO MAPNO case for a specific map, remove
            if (current_map == 46)
            {
                fname = "chu_ele.smk";
                pinfo = &game_dirs[DirPlace_LangData];
                if (game_dirs[DirPlace_Data].use_cd == 1)
                    sprintf(fnbuf, "%s%s/%s", cd_drive, pinfo->directory, fname);
                else
                    sprintf(fnbuf, "data/%s", fname);
            }
            else
            {
                fnbuf[0] = '\0';
            }
            break;
        default:
            fnbuf[0] = '\0';
            break;
        }
        break;
    case MPly_MissiFail:
        switch (background_type)
        {
        case 0:
            fname = "syn_fail.smk";
            sprintf(fnbuf, "qdata/%s", fname);
            break;
        case 1:
            fname = "syn_fail.smk";
            sprintf(fnbuf, "qdata/%s", fname);
            break;
        default:
            fnbuf[0] = '\0';
            break;
        }
        break;
    case MPly_GameOver:
        switch (background_type)
        {
        case 0:
            fname = "syn_go.smk";
            sprintf(fnbuf, "qdata/%s", fname);
            break;
        case 1:
            fname = "zel-go.smk";
            sprintf(fnbuf, "qdata/%s", fname);
            break;
        default:
            fnbuf[0] = '\0';
            break;
        }
        break;
    case MPly_Outro:
        p_campgn = &campaigns[background_type];
        //TODO should support use_cd like intro
        sprintf(fnbuf, "%s", p_campgn->OutroFMV);
        break;
    default:
        fnbuf[0] = '\0';
        break;
    }
}

static void clear_smacker_skip_keys(void)
{
    clear_key_pressed(KC_SPACE);
    clear_key_pressed(KC_RETURN);
    clear_key_pressed(KC_ESCAPE);
}

void play_smacker(ushort vid_type)
{
    char fname[FILENAME_MAX];
    u32 plyflags;
    TbScreenMode scr_md_fmvid;
    TbBool prepare_draw_on_last_frame;

    update_danger_music(2);

    sprint_fmv_filename(vid_type, fname, sizeof(fname));

    // For outro, last frame of the video should stay on screen
    // and the screen should be prepared for further drawing
    prepare_draw_on_last_frame = (vid_type == MPly_Outro);

    if ((vid_type == MPly_Intro) || (vid_type == MPly_Outro)) {
        scr_md_fmvid = screen_mode_fmvid_lo;
    } else {
        scr_md_fmvid = screen_mode_fmvid_hi;
    }

    if (prepare_draw_on_last_frame) {
        // If we need to prepare drawing after, do a full mode change
        if (lbDisplay.ScreenMode != scr_md_fmvid)
            setup_screen_mode(scr_md_fmvid);
    } else {
        // Do simple mode change, we will switch again after the video
        // also skip the mode change entirely if no video to play
        if ((lbDisplay.ScreenMode != scr_md_fmvid) && (fname[0] != '\0'))
            setup_simple_screen_mode(scr_md_fmvid);
    }
    LbMouseChangeSprite(NULL);

    plyflags = 0;

    if (fname[0] != '\0') {
        show_black_screen();
        clear_smacker_skip_keys();
        play_smk(fname, 13, plyflags);
        smack_malloc_free_all();
    }

    if (prepare_draw_on_last_frame) {
            struct Campaign *p_campgn;
            short raw_w, raw_h;
            short x, y;
            raw_w = 300;
            raw_h = 150;
            x = (lbDisplay.GraphicsScreenWidth - raw_w) / 2;
            y = (lbDisplay.GraphicsScreenHeight - raw_h) / 2 - 1;

            p_campgn = &campaigns[background_type];
            LbScreenClear(0);
            cover_screen_rect_with_raw_file(x, y, raw_w, raw_h, p_campgn->OutroBkFn);

            ingame_palette_load(0);
            LbScreenWaitVbi();
            LbPaletteSet(display_palette);
            swap_wscreen();
    } else {
        show_black_screen();
    }
    game_hacky_update();
    clear_smacker_skip_keys();
}

void play_smacker_then_back_to_engine(ushort vid_type)
{
    TbScreenMode bkpmode;

    bkpmode = lbDisplay.ScreenMode;
    play_smacker(vid_type);
    setup_screen_mode(bkpmode);
}

void play_smacker_then_back_to_menu(ushort vid_type)
{
    stop_sample_using_heap(0, 122);
    stop_sample_using_heap(0, 122);

    play_smacker(vid_type);
    setup_screen_mode(screen_mode_menu);

    play_sample_using_heap(0, 122, FULL_VOL, EQUL_PAN, NORM_PTCH, LOOP_4EVER, 3);
    play_sample_using_heap(0, 122, FULL_VOL, EQUL_PAN, NORM_PTCH, LOOP_4EVER, 3);
}

void play_intro(void)
{
    LOGSYNC("Starting");
    if ( (cmdln_param_bcg || is_single_game) &&
      ((ingame.Flags & GamF_SkipIntro) == 0))
    {
        // After this function, we return either to engine or to menu,
        // depending on how the app was started
        if (cmdln_param_bcg)
            play_smacker_then_back_to_menu(MPly_Intro);
        else
            play_smacker_then_back_to_engine(MPly_Intro);
    }
}

/** Primitive but very fast mem allocation.
 */
void *opti_smack_malloc(uint32_t msize)
{
    void *ptr;

    assert(smack_malloc_used_tot + msize < scratch_malloc_size);
    ptr = scratch_malloc_mem + smack_malloc_used_tot;
    smack_malloc_used_tot += msize;
    return ptr;
}

void opti_smack_mfree(void *ptr)
{
    // Do not free any memory - we have enough space to play one video
}

void smack_malloc_free_all(void)
{
    smack_malloc_used_tot = 0;
}

void smack_malloc_setup(void)
{
    smack_malloc_used_tot = 0;
    set_smack_malloc(opti_smack_malloc);
    set_smack_free(opti_smack_mfree);
}

/******************************************************************************/
