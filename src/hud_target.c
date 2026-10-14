/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file hud_target.c
 *     Ingame Heads-Up Display targetted thing draw.
 * @par Purpose:
 *     Implement functions drawing the HUD elements around target to be
 *     attacked.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 Aug 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "hud_target.h"

#include "bfbox.h"
#include "bfgentab.h"
#include "bfline.h"
#include "bfsprite.h"
#include "insspr.h"
#include <stdlib.h>

#include "engincam.h"
#include "engincolour.h"
#include "enginsngobjs.h"
#include "engintrns.h"
#include "huddrwlstm.h"

#include "bigmap.h"
#include "engindrwlstm_wrp.h"
#include "engindrwlstx.h"
#include "frame_sprani.h"
#include "game_sprts.h"
#include "game.h"
#include "mouse.h"
#include "player.h"
#include "sound.h"
#include "thing.h"
#include "vehicle.h"
#include "weapon.h"
#include "swlog.h"
/******************************************************************************/
static const ushort target_person_frame_elements_sprno[] = { 78, 79, 81, 80 };
struct TbSprite *target_person_frame_elements_spr[5];

static const ushort target_vehicle_frame_elements_sprno[] = { 84, 85, 87, 86 };
struct TbSprite *target_vehicle_frame_elements_spr[5];

TbBool hud_show_target_health = false;

s32 target_old_frameno = 0;

short goto_point_frame_no = 0;
short goto_point_frame_count = 0;

/******************************************************************************/

void show_goto_point(u32 flag)
{
    ushort frame_count;
    short frm;
    struct Thing *p_thing;
    short face;
    ThingIdx dcthing;

    if (flag & 0xff)
    {
        goto_point_frame_count = 0;
        goto_point_frame_no = nstart_ani[926];
        return;
    }
    frm = goto_point_frame_no;
    if (frm == 0) {
        return;
    }
    frame_count = goto_point_frame_count++;
    if (frame_count > 5)
        goto_point_frame_no = 0;
    else
        goto_point_frame_no = frame[goto_point_frame_no].Next;

    dcthing = players[local_player_no].DirectControl[mouser];
    p_thing = &things[dcthing];
    if ((((p_thing->Flag & TngF_InVehicle) != 0) || (p_thing->State == PerSt_GOTO_POINT)) &&
      ((p_thing->Flag2 & TgF2_Unkn0040) == 0))
    {
        int height;
        int cor_x, cor_y, cor_z;
        TbPixel colour;

        if (goto_point_frame_count == 1)
            play_sample_using_heap(0, 92, 127, 64, 100, 0, 3u);

        colour = 0;
        cor_x = p_thing->U.UPerson.GotoX;
        cor_z = p_thing->U.UPerson.GotoZ;
        face = players[local_player_no].GotoFace;
        if (face != 0)
        {
            int prc_x, prc_z;

            prc_x = MAPCOORD_TO_PRCCOORD(cor_x,0);
            prc_z = MAPCOORD_TO_PRCCOORD(cor_z,0);
            if (face <= 0)
                height = get_height_on_face_quad(prc_x, prc_z, -face);
            else
                height = get_height_on_face(prc_x, prc_z, face);
        }
        else
        {
            height = alt_at_point(cor_x, cor_z);
        }

        cor_y = PRCCOORD_TO_MAPCOORD(height);
        if ((p_thing->Flag2 & TgF2_Unkn00080000) != 0)
            colour = 48;
        enlist_hud_draw_mapcoord_frame_one_colour(cor_x, cor_y, cor_z, frm, 0, colour);
    }
}

void draw_hud_target_mouse(ThingIdx dcthing)
{
    PlayerInfo *p_locplayer;
    struct Thing *p_dcthing;

    p_dcthing = &things[dcthing];
    p_locplayer = &players[local_player_no];
    if (p_locplayer->Target > 0)
    {
        struct Thing *p_targtng;
        int weprange;
        ushort msspr;
        uint range;

        weprange = current_hand_weapon_range(p_dcthing);
        switch (p_locplayer->TargetType)
        {
        case TrgTp_Unkn1:
        case TrgTp_Unkn2:
        case TrgTp_Unkn6:
        case TrgTp_Unkn7:
            p_locplayer->field_102 = p_locplayer->Target;
            p_locplayer->TargetType = TrgTp_Unkn7;
            p_targtng = &things[p_locplayer->Target];
            range = weprange * weprange;
            if (can_i_see_thing(p_dcthing, p_targtng, range, 3) ) {
                msspr = 3;
            } else {
                msspr = 2;
            }
            do_change_mouse(msspr);
            break;
        case TrgTp_Unkn3:
            p_locplayer->field_102 = p_locplayer->Target;
            do_change_mouse(7);
            break;
        case TrgTp_Unkn4:
            p_locplayer->field_102 = p_locplayer->Target;
            p_targtng = &things[p_locplayer->field_102];
            p_dcthing = &things[p_locplayer->DirectControl[mouser]];
            if (can_i_enter_vehicle(p_dcthing, p_targtng)) {
              msspr = 6;
            } else {
              range = p_targtng->Radius * p_targtng->Radius + weprange * weprange;
              if (can_i_see_thing(p_dcthing, p_targtng, range, 3) ) {
                msspr = 3;
              } else {
                msspr = 2;
              }
            }
            do_change_mouse(msspr);
            break;
        default:
            break;
        }
    }
    else if (p_locplayer->Target < 0)
    {
        if (p_locplayer->TargetType == TrgTp_Unkn3) {
          p_locplayer->field_102 = p_locplayer->Target;
          do_change_mouse(7);
        } else {
          p_locplayer->field_102 = p_locplayer->Target;
          do_change_mouse(5);
        }
    }
    else
    {
        do_change_mouse(8);
    }
}

void init_draw_target(void)
{
    if (target_old_frameno == 0)
        target_old_frameno = nstart_ani[983];
}

void draw_hud_lock_target(void)
{
    struct Thing *p_dcthing;
    struct Thing *p_vehicle;
    struct Thing *p_target;
    int VX, VY_m8, VZ;
    int tgtimer;

    p_dcthing = &things[players[local_player_no].DirectControl[0]];
    if ((p_dcthing->Flag & TngF_InVehicle) == 0) {
        return;
    }

    p_vehicle = &things[p_dcthing->U.UPerson.Vehicle];
    if ((p_vehicle->SubType != SubTT_VEH_TANK) &&
      (p_vehicle->SubType != SubTT_VEH_MECH)) {
        return;
    }

    p_target = p_dcthing->PTarget;
    if (p_target != NULL) // Invalid pointer correction
    {
        if ((p_target <= &things[0]) || (p_target >= &things[THINGS_LIMIT])) {
            LOGERR("Tried to shoot at invalid target 0x%p", p_target);
            p_dcthing->PTarget = p_target = NULL;
        }
    }

    if (p_target != NULL)
    {
        VX = PRCCOORD_TO_MAPCOORD(p_target->X);
        VY_m8 = PRCCOORD_TO_YCOORD(p_target->Y) + 120;
        VZ = PRCCOORD_TO_MAPCOORD(p_target->Z);
    }
    else if ((p_dcthing->Flag & TngF_ShootAtPos) == 0)
    {
        return;
    }
    else if ((p_dcthing->Flag & TngF_PlayerAgent) != 0)
    {
        PlayerIdx plyr;
        ushort plagent;

        plyr = p_dcthing->U.UPerson.ComCur >> 2;
        plagent = p_dcthing->U.UPerson.ComCur & 3;
        VX = players[plyr].UserVX[plagent];
        VY_m8 = 8 * players[plyr].UserVY[plagent];
        VZ = players[plyr].UserVZ[plagent];
    }
    else
    {
        VX = p_dcthing->VX;
        VY_m8 = 8 * p_dcthing->VY;
        VZ = p_dcthing->VZ;
    }

    tgtimer = p_vehicle->OldTarget;
    if (tgtimer < 2048)
    {
        short rctsize, lnlen, lnthick;
        TbPixel colour;

        if (tgtimer >= 24) {
            rctsize = tgtimer >> 1;
            colour = colour_lookup[4];
        } else {
            rctsize = 12;
            colour = colour_lookup[2];
        }
        rctsize = (rctsize * overall_scale) >> 8;
        lnthick = (1 * overall_scale) >> 8;
        if (lnthick < 1)
            lnthick = 1;
        lnlen = (8 * overall_scale) >> 8;

         enlist_hud_draw_mapcoord_corner_rect(VX, VY_m8, VZ, rctsize, rctsize,
           lnlen, lnthick, 0, colour);
    }
}

void draw_hud_old_frame_health_bar(int x, int y, struct Thing *p_thing, short space)
{
    int dx, dy;
    int hp_per_px, val_cur, val_max;
    int h_total, h_cur, h_ext, w;
    ushort drwflags;
    TbPixel colour;

    dx = (((space >> 1) + 2) * overall_scale) >> 8;
    dy = (10 * overall_scale) >> 8;
    h_total = -(15 * overall_scale) >> 8;
    w = (2 * overall_scale) >> 8;

    if (p_thing->Type == TT_VEHICLE)
        val_max = p_thing->U.UVehicle.MaxHealth;
    else
        val_max = p_thing->U.UPerson.MaxHealth;
    hp_per_px = val_max / dy;
    if (hp_per_px == 0)
        hp_per_px = 1;

    if (p_thing->Health <= val_max)
        val_cur = p_thing->Health;
    else
        val_cur = val_max;

    h_cur = val_cur / hp_per_px;
    if (h_cur > dy)
        h_cur = dy;

    drwflags = Lb_SPRITE_TRANSPAR4;
    colour = colour_lookup[ColLU_BLUE];
    enlist_hud_draw_box(dx + x, y + h_total, w, dy,
      drwflags, 32, colour);

    if (p_thing->Health >= 0)
    {
        drwflags = 0;
        if (h_cur < 3)
            colour = colour_lookup[ColLU_RED];
        enlist_hud_draw_box(x + dx, y + dy + h_total - h_cur, w, h_cur,
          drwflags, 32, colour);

        // Extra health if the thing is above max (soul gun bonus)
        if (p_thing->Health > val_max)
        {
            h_ext = (p_thing->Health - val_max) / hp_per_px;
            if (h_ext > dy)
                h_ext = dy;

            drwflags = Lb_SPRITE_TRANSPAR4;
            colour = colour_lookup[ColLU_YELLOW];
            enlist_hud_draw_box(dx + x, y + h_total, w, dy,
              drwflags, 32, colour);

            drwflags = 0;
            enlist_hud_draw_box(x + dx, y + dy + h_total - h_ext, w, h_ext,
              drwflags, 32, colour);
        }
    }
}

void draw_hud_old_frame_shield_bar(int x, int y, struct Thing *p_thing, short space)
{
    int dx, dy;
    int sp_per_px;
    int h_total, h_cur, w;
    ushort drwflags;
    TbPixel colour;

    dx = (((space >> 1) + 7) * overall_scale) >> 8;
    dy = (10 * overall_scale) >> 8;
    h_total = -(20 * overall_scale) >> 8;
    w = (2 * overall_scale) >> 8;

    sp_per_px = p_thing->U.UPerson.MaxShieldEnergy / dy;
    if (sp_per_px == 0)
        sp_per_px = 1;

    h_cur = p_thing->U.UPerson.ShieldEnergy / sp_per_px;
    if (h_cur > dy)
      h_cur = dy;

    colour = colour_lookup[ColLU_BLUE];
    drwflags = Lb_SPRITE_TRANSPAR4;
    enlist_hud_draw_box(dx + x, h_total + y, w, dy,
      drwflags, 32, colour);

    if (p_thing->U.UPerson.ShieldEnergy > 0)
    {
        drwflags = 0;
        if (h_cur < 3)
            colour = colour_lookup[ColLU_RED];
        enlist_hud_draw_box(x + dx, y + dy + h_total - h_cur, w, h_cur,
          drwflags, 32, colour);
    }
}

void draw_hud_target_old_frame(struct Thing *p_target, int frm, short space)
{
    struct EnginePoint ep;

    ep.X3d = PRCCOORD_TO_MAPCOORD(p_target->X) - engn_xc;
    ep.Y3d = PRCCOORD_TO_YCOORD(p_target->Y); // subtracted within transform_point(): - 8 * engn_yc;
    ep.Z3d = PRCCOORD_TO_MAPCOORD(p_target->Z) - engn_zc;
    ep.Flags = 0;
    transform_point(&ep);

    int sh_x;
    ubyte brig;

    sh_x = (space * overall_scale) >> 9;
    brig = PALETTE_FADE_LEVELS / 2;

    enlist_hud_draw_frame_alpha(ep.pp.X - sh_x, ep.pp.Y, frm +  0, 0, overall_scale, brig);
    enlist_hud_draw_frame_alpha(ep.pp.X + sh_x, ep.pp.Y, frm + 10, 0, overall_scale, brig);

    draw_hud_old_frame_health_bar(ep.pp.X, ep.pp.Y, p_target, space);
    draw_hud_old_frame_shield_bar(ep.pp.X, ep.pp.Y, p_target, space);
}

void draw_target_person(struct Thing *p_person, int radius)
{
    int cor_x, cor_y_m8, cor_z;
    int i, n;

    if ((p_person->Flag & TngF_Destroyed) != 0)
        return;

    if (hud_show_target_health) {
        draw_hud_target_old_frame(p_person, target_old_frameno, 8 + radius);
        target_old_frameno = frame[target_old_frameno].Next;
        return;
    }

    //TODO maybe initialize this during sprite loading?
    n = sizeof(target_person_frame_elements_sprno)/sizeof(target_person_frame_elements_sprno[0]);
    for (i = 0; i < n; i++) {
        target_person_frame_elements_spr[i] = &pop1_sprites[target_person_frame_elements_sprno[i]];
    }
    target_person_frame_elements_spr[n] = NULL;

    cor_x = PRCCOORD_TO_MAPCOORD(p_person->X);
    // TODO Why constant height? Maybe differnt main body position for different thing types?
    cor_y_m8 = PRCCOORD_TO_YCOORD(p_person->Y) + 8 * 15;
    cor_z = PRCCOORD_TO_MAPCOORD(p_person->Z);

    enlist_hud_draw_mapcoord_sprites_in_quarters_unscaled(cor_x,
      cor_y_m8, cor_z, target_person_frame_elements_spr, 0, radius);
}

void draw_target_vehicle(struct Thing *p_vehicle)
{
    int cor_x, cor_y_m8, cor_z;
    int radius;
    int i, n;

    if (hud_show_target_health) {
        radius = (p_vehicle->Radius >> 4) + (p_vehicle->Radius >> 5);
        draw_hud_target_old_frame(p_vehicle, target_old_frameno, 8 + radius);
        target_old_frameno = frame[target_old_frameno].Next;
        return;
    }

    //TODO maybe initialize this during sprite loading?
    n = sizeof(target_vehicle_frame_elements_sprno)/sizeof(target_vehicle_frame_elements_sprno[0]);
    for (i = 0; i < n; i++) {
        target_vehicle_frame_elements_spr[i] = &pop1_sprites[target_vehicle_frame_elements_sprno[i]];
    }
    target_vehicle_frame_elements_spr[n] = NULL;

    cor_x = PRCCOORD_TO_MAPCOORD(p_vehicle->X);
    cor_y_m8 = PRCCOORD_TO_YCOORD(p_vehicle->Y);
    cor_z = PRCCOORD_TO_MAPCOORD(p_vehicle->Z);
    radius = p_vehicle->Radius >> 4;

    {
        int cor_lr, cor_sm;
        if (abs(cor_x - engn_xc) <= abs(cor_z - engn_zc)) {
            cor_sm = abs(cor_x - engn_xc);
            cor_lr = abs(cor_z - engn_zc);
        } else {
            cor_sm = abs(cor_z - engn_zc);
            cor_lr = abs(cor_x - engn_xc);
        }
        if (cor_lr + (cor_sm >> 1) > TILE_TO_MAPCOORD(20,0))
            return;
    }

    enlist_hud_draw_mapcoord_sprites_in_quarters_unscaled(cor_x,
      cor_y_m8, cor_z, target_vehicle_frame_elements_spr, 0, radius);
}

void draw_hud_target2(short dcthing, short target)
{
    struct Thing *p_dcthing;
    struct Thing *p_target;

    p_dcthing = &things[dcthing];
    p_target = &things[target];

    if (current_weapon_has_targetting(p_dcthing))
    {
        struct Thing *p_dctarget;
        p_dctarget = p_dcthing->PTarget;
        if ((p_dctarget != NULL) && ((p_dctarget->Flag & TngF_Destroyed) == 0))
        {
            int sz;

            sz = 4 * (18 - p_dcthing->U.UPerson.WeaponTimer);
            if (sz < 6)
                sz = 6;
            draw_target_person(p_dctarget, sz);
        }
    }
    else
    {
        struct Thing *p_dctarget;
        switch (p_target->Type)
        {
        case TT_VEHICLE:
            draw_target_vehicle(p_target);
            break;
        case TT_PERSON:
            if ((p_target->Flag & TngF_InVehicle) != 0) {
                p_dctarget = &things[p_target->U.UPerson.Vehicle];
            } else {
                p_dctarget = p_target;
            }
            draw_target_person(p_dctarget, 2);
            break;
        }
    }
}

/******************************************************************************/
