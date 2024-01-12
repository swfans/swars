/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file lvfiles.c
 *     Routines for level and map files handling.
 * @par Purpose:
 *     Implement functions for handling and checking level and map files.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "lvfiles.h"

#include <assert.h>
#include <string.h>
#include "bffile.h"
#include "triangls.h"
#include "trpoints.h"
#include "trstate.h"
#include "pathtrig.h"
#include "bigmap.h"
#include "enginlights.h"
#include "enginsngobjs.h"
#include "enginsngtxtr.h"
#include "enginsngobjs.h"
#include "game.h"
#include "game_data.h"
#include "matrix.h"
#include "building.h"
#include "thing.h"
#include "vehtraffic.h"
#include "swlog.h"
/******************************************************************************/

extern struct QuickLoad quick_load_pc[19];


void load_map_dat_pc_handle(TbFileHandle fh)
{
    ulong fmtver;
    ushort num_sthings, num_things;
    short i;

    LbFileRead(fh, &fmtver, sizeof(fmtver));
    if (fmtver >= 19)
    {
        assert(sizeof(struct MyMapElement) == 18);
        LbFileRead(fh, game_my_big_map, sizeof(struct MyMapElement) * 128 * 128);
    }
    else
    {
        struct MyMapElementOldV7 old_my_map_el;
        assert(sizeof(old_my_map_el) == 24);
        for (i = 0; i < 128 * 128; i++) {
            LbFileRead(fh, &old_my_map_el, sizeof(old_my_map_el));
            refresh_old_my_big_map_format(&game_my_big_map[i], &old_my_map_el, fmtver);
        }
    }
    {
        LbFileRead(fh, &next_floor_texture, sizeof(next_floor_texture));
        LbFileRead(fh, game_textures, sizeof(struct SingleFloorTexture) * next_floor_texture);
    }
    {
        LbFileRead(fh, &next_face_texture, sizeof(next_face_texture));
        LbFileRead(fh, game_face_textures, sizeof(struct SingleTexture) * next_face_texture);
    }
    {
        LbFileRead(fh, &next_object_point, sizeof(next_object_point));
        LbFileRead(fh, game_object_points, sizeof(struct SinglePoint) * next_object_point);
    }
    if (fmtver >= 19)
    {
        LbFileRead(fh, &next_object_face, sizeof(next_object_face));
        assert(sizeof(struct SingleObjectFace3) == 32);
        LbFileRead(fh, game_object_faces, sizeof(struct SingleObjectFace3) * next_object_face);
    }
    else
    {
        struct SingleObjectFace3OldV7 old_object_face;
        LbFileRead(fh, &next_object_face, sizeof(next_object_face));
        assert(sizeof(old_object_face) == 48);
        for (i = 0; i < next_object_face; i++) {
            LbFileRead(fh, &old_object_face, sizeof(old_object_face));
            refresh_old_object_face_format(&game_object_faces[i], &old_object_face, fmtver);
        }
    }
    {
        LbFileRead(fh, &next_object, sizeof(next_object));
        LbFileRead(fh, game_objects, sizeof(struct SingleObject) * next_object);
    }
    {
        LbFileRead(fh, &next_quick_light, sizeof(next_quick_light));
        LbFileRead(fh, game_quick_lights, sizeof(struct QuickLight) * next_quick_light);
    }
    if (fmtver >= 14)
    {
        LbFileRead(fh, &next_full_light, sizeof(next_full_light));
        assert(sizeof(struct FullLight) == 32);
        LbFileRead(fh, game_full_lights, sizeof(struct FullLight) * next_full_light);
    }
    else
    {
        struct FullLightOldV12 old_full_light;
        LbFileRead(fh, &next_full_light, sizeof(next_full_light));
        assert(sizeof(old_full_light) == 40);
        for (i = 0; i < next_full_light; i++) {
            LbFileRead(fh, &old_full_light, sizeof(old_full_light));
            refresh_old_full_light_format(&game_full_lights[i], &old_full_light, fmtver);
        }
    }
    {
        LbFileRead(fh, &next_normal, sizeof(next_normal));
        LbFileRead(fh, game_normals, sizeof(struct Normal) * next_normal);
    }
    if (fmtver >= 19)
    {
        LbFileRead(fh, &next_object_face4, sizeof(next_object_face4));
        assert(sizeof(struct SingleObjectFace4) == 40);
        LbFileRead(fh, game_object_faces4, sizeof(struct SingleObjectFace4) * next_object_face4);
    }
    else if (fmtver >= 7)
    {
        struct SingleObjectFace4OldV7 old_object_face4;
        LbFileRead(fh, &next_object_face4, sizeof(next_object_face4));
        assert(sizeof(old_object_face4) == 60);
        for (i = 0; i < next_object_face4; i++) {
            LbFileRead(fh, &old_object_face4, sizeof(old_object_face4));
            refresh_old_object_face4_format(&game_object_faces4[i], &old_object_face4, fmtver);
        }
    }
    else
    {
        next_object_face4 = 1;
    }
    LOGSYNC("stats: object_faces=%hu objects=%hu quick_lights=%hu full_lights=%hu normals=%hu object_faces4=%hu",
      next_object_face, next_object, next_quick_light, next_full_light, next_normal, next_object_face4);

    clear_mapwho_on_whole_map();

    if (fmtver >= 5)
    {
        struct SimpleThing loc_sthing;

        LbFileRead(fh, &num_sthings, sizeof(num_sthings));
        assert(sizeof(struct SimpleThing) == 60);
        for (i = num_sthings - 1; i != -1; i--)
        {
            LbFileRead(fh, &loc_sthing, sizeof(struct SimpleThing));
            switch (loc_sthing.Type)
            {
            case TT_UNKN10:
                new_thing_type10_clone(&loc_sthing);
                break;
            case SmTT_SMOKE_GENERATOR:
                new_thing_smoke_gen_clone(&loc_sthing);
                break;
            case SmTT_STATIC:
                new_thing_static_clone(&loc_sthing);
                break;
            default:
                {
                  char locbuf[256];
                  snprint_sthing(locbuf, sizeof(locbuf), &loc_sthing);
                  LOGWARN("Discarded: %s", locbuf);
                }
                break;
            }
        }
    }
    else
    {
        num_sthings = 0;
    }

    if (fmtver >= 6)
    {
        LbFileRead(fh, &next_anim_tmap, sizeof(next_anim_tmap));
        assert(sizeof(struct AnimTmap) == 54);
        LbFileRead(fh, game_anim_tmaps, sizeof(struct AnimTmap) * next_anim_tmap);
    }
    else
    {
        next_anim_tmap = 1;
        game_anim_tmaps[1].Texture = 0;
    }

    if (fmtver >= 19)
    {
        ushort num_things;
        struct Thing loc_thing;
        struct M33 loc_mat;
        short shut_h;

        shut_h = 100;
        LbFileRead(fh, &num_things, sizeof(num_things));
        assert(sizeof(struct Thing) == 168);
        for (i = num_things - 1; i > -1; i--)
        {
            LbFileRead(fh, &loc_thing, sizeof(struct Thing));
            if (loc_thing.U.UObject.Object <= 0) {
                char locbuf[256];
                snprint_thing(locbuf, sizeof(locbuf), &loc_thing);
                LOGWARN("Bad object %d in %s",
                  (int)loc_thing.U.UObject.Object, locbuf);
                continue;
            }
            switch (loc_thing.SubType)
            {
            case SubTT_BLD_36:
            case SubTT_BLD_37:
                LbFileRead(fh, &loc_mat, sizeof(struct M33));
                new_thing_building_clone(&loc_thing, &loc_mat, shut_h);
                break;
            default:
                new_thing_building_clone(&loc_thing, NULL, shut_h);
                break;
            }
        }
    }
    else if (fmtver >= 8)
    {
        struct Thing loc_thing;
        struct ThingOldV9 old_thing;

        LbFileRead(fh, &num_things, sizeof(num_things));
        assert(sizeof(old_thing) == 216);
        for (i = num_things - 1; i > -1; i--)
        {
            struct SingleObject *p_sobj;

            LbFileRead(fh, &old_thing, sizeof(old_thing));
            //TODO map fmtver is unlikely to match level fmtver
            refresh_old_thing_format(&loc_thing, &old_thing, fmtver);
            if (loc_thing.U.UObject.Object <= 0) {
                char locbuf[256];
                snprint_thing(locbuf, sizeof(locbuf), &loc_thing);
                LOGWARN("Bad object %d in %s",
                  (int)loc_thing.U.UObject.Object, locbuf);
                continue;
            }
            p_sobj = &game_objects[loc_thing.U.UObject.Object];
            if (p_sobj->OffsetY == 32000) {
                continue;
            }
            new_thing_building_clone(&loc_thing, NULL, 100);
        }
    }
    else
    {
        num_things = 0;
    }

    if (fmtver >= 6)
    {
        LbFileRead(fh, &next_traffic_node, sizeof(next_traffic_node));
        assert(sizeof(struct TrafficNode) == 36);
        LbFileRead(fh, game_traffic_nodes, sizeof(struct TrafficNode) * next_traffic_node);
    }
    else
    {
        next_traffic_node = 1;
    }

    if (fmtver >= 12)
    {
        LbFileRead(fh, &next_light_command, sizeof(next_light_command));
        assert(sizeof(struct LightCommand) == 36);
        LbFileRead(fh, game_light_commands, sizeof(struct LightCommand) * next_light_command);
    }
    else
    {
       next_light_command = 1;
    }
    if (fmtver >= 13)
    {
        LbFileRead(fh, &next_bezier_pt, sizeof(next_bezier_pt));
        assert(sizeof(struct BezierPt) == 28);
        LbFileRead(fh, bezier_pts, sizeof(struct BezierPt) * next_bezier_pt);
    }
    else
    {
        next_bezier_pt = 1;
    }
    LOGSYNC("stats: sthings=%hu things=%hu traffic_nodes=%hu light_commands=%hu bezier_pts=%hu",
      num_sthings, num_things, next_traffic_node, next_light_command, next_bezier_pt);
}

void load_mad_pc_buffer(ubyte *mad_ptr, long rdsize)
{
    short shut_h;
    ulong fmtver;
    short i;

    shut_h = 100;
    fmtver = *(ulong *)mad_ptr;
    mad_ptr += 4;

    if (fmtver != 1) {
        LOGWARN("Unexpected MAD version %lu", fmtver);
    }

    // Set amounts of quick_load array items
    for (i = 0; quick_load_pc[i].Size != 0; i++)
    {
        ushort *p_numb;
        p_numb = quick_load_pc[i].Numb;
        if (p_numb != NULL) {
            *p_numb = *(ushort *)mad_ptr;
            mad_ptr += 2;
        }
    }
    // Set data pointers within the quick_load items
    for (i = 0; quick_load_pc[i].Size != 0; i++)
    {
        int entsize, nentries;
        ushort *p_numb;
        *quick_load_pc[i].Ptr = mad_ptr;
        p_numb = quick_load_pc[i].Numb;
        if (p_numb != NULL) {
            entsize = quick_load_pc[i].Size;
            nentries = quick_load_pc[i].Extra + *p_numb;
        } else {
            nentries = quick_load_pc[i].Size;
            entsize = quick_load_pc[i].Extra;
        }
        mad_ptr += nentries * entsize + 2;
    }
    // Update mem_game[] to the new amounts
    for (i = 1; i < 17; i++)
    {
        ushort *p_numb;
        p_numb = quick_load_pc[i].Numb;
        mem_game[i].N = quick_load_pc[i].Extra + *p_numb;
    }

    memcpy(&selected_triangulation_no, mad_ptr, sizeof(selected_triangulation_no));
    mad_ptr += sizeof(selected_triangulation_no);
    memcpy(&triangulation_initied, mad_ptr, sizeof(triangulation_initied));
    mad_ptr += sizeof(triangulation_initied);
    assert(sizeof(struct Triangulation) == 60);
    memcpy(triangulation, mad_ptr, sizeof(struct Triangulation) * 4);
    mad_ptr += sizeof(struct Triangulation) * 4;
    assert(sizeof(struct TrTriangle) == 16);
    triangulation[0].Triangles = (struct TrTriangle *)mad_ptr;
    mad_ptr += sizeof(struct TrTriangle) * triangulation[0].max_Triangles;
    triangulation[1].Triangles = (struct TrTriangle *)mad_ptr;
    mad_ptr += sizeof(struct TrTriangle) * triangulation[1].max_Triangles;
    triangulation[2].Triangles = (struct TrTriangle *)mad_ptr;
    mad_ptr += sizeof(struct TrTriangle) * triangulation[2].max_Triangles;
    triangulation[3].Triangles = (struct TrTriangle *)mad_ptr;
    mad_ptr += sizeof(struct TrTriangle) * triangulation[3].max_Triangles;
    assert(sizeof(struct TrPoint) == 8);
    triangulation[0].Points = (struct TrPoint *)mad_ptr;
    mad_ptr += sizeof(struct TrPoint) * triangulation[0].max_Points;
    triangulation[1].Points = (struct TrPoint *)mad_ptr;
    mad_ptr += sizeof(struct TrPoint) * triangulation[1].max_Points;
    triangulation[2].Points = (struct TrPoint *)mad_ptr;
    mad_ptr += sizeof(struct TrPoint) * triangulation[2].max_Points;
    triangulation[3].Points = (struct TrPoint *)mad_ptr;
    mad_ptr += sizeof(struct TrPoint) * triangulation[3].max_Points;

    clear_mapwho_on_whole_map();

    ushort num_sthings;
    struct SimpleThing *p_clsthing;

    dword_177750 = mad_ptr;
    num_sthings = *(ushort *)mad_ptr;
    mad_ptr += 2;
    for (i = num_sthings - 1; i > -1; i--)
    {
        p_clsthing = (struct SimpleThing *)mad_ptr;
        mad_ptr += sizeof(struct SimpleThing);
        switch (p_clsthing->Type)
        {
        case TT_UNKN10:
            new_thing_type10_clone(p_clsthing);
            break;
        case SmTT_SMOKE_GENERATOR:
            new_thing_smoke_gen_clone(p_clsthing);
            break;
        case SmTT_STATIC:
            new_thing_static_clone(p_clsthing);
            break;
        default:
            {
                char locbuf[256];
                snprint_sthing(locbuf, sizeof(locbuf), p_clsthing);
                LOGWARN("Discarded: %s", locbuf);
            }
            break;
        }
    }

    ushort num_things;
    struct Thing *p_clthing;

    num_things = *(ushort *)mad_ptr;
    mad_ptr += 2;
    for (i = num_things - 1; i > -1; i--)
    {
        p_clthing = (struct Thing *)mad_ptr;
        mad_ptr += sizeof(struct Thing);
        if (p_clthing->U.UObject.Object <= 0)
            continue;
        switch (p_clthing->SubType)
        {
        case SubTT_BLD_36:
        case SubTT_BLD_37:
            new_thing_building_clone(p_clthing, (struct M33 *)mad_ptr, shut_h);
            mad_ptr += sizeof(struct M33);
            break;
        default:
            new_thing_building_clone(p_clthing, NULL, shut_h);
            break;
        }
    }

    if (mad_ptr - (ubyte *)dword_177750 >= 100000)
        unkn_mech_arr7 = mad_ptr;
    else
        unkn_mech_arr7 = dword_177750 + 100000;
}

TbResult load_map_dat(ushort mapno)
{
    char dat_fname[52];
    TbFileHandle fh;

    next_local_mat = 1;

    sprintf(dat_fname, "%s/map%03d.dat", "maps", mapno);
    fh = LbFileOpen(dat_fname, Lb_FILE_MODE_READ_ONLY);
    if (fh == INVALID_FILE) {
        return Lb_FAIL;
    }
    load_map_dat_pc_handle(fh);
    LbFileClose(fh);

    return Lb_SUCCESS;
}

TbResult load_map_mad(ushort mapno)
{
    char mad_fname[52];
    long fsize;

    next_local_mat = 1;

    sprintf(mad_fname, "%s/map%03d.mad", "maps", mapno);
    fsize = LbFileLoadAt(mad_fname, scratch_malloc_mem);
    if (fsize == Lb_FAIL)
        return Lb_FAIL;

    load_mad_pc_buffer(scratch_malloc_mem, fsize);

    unkn_buildings_processing();
    unkn_lights_processing();
    triangulation_select(1);

    return Lb_SUCCESS;
}

void load_map_bnb(int a1)
{
    asm volatile ("call ASM_load_map_bnb\n"
        : : "a" (a1));
}

TbResult load_mad_pc(ushort mapno)
{
#if 0
    asm volatile ("call ASM_load_mad_pc\n"
        : : "a" (mapno));
    return Lb_SUCCESS;
#else
    TbResult ret;

    ingame.Flags |= GamF_Unkn00010000;
    init_free_explode_faces();
    if (mapno != 0) {
        load_map_bnb(mapno);
        ret = load_map_mad(mapno);
    } else {
        ret = Lb_OK;
    }
    sub_73C64("", 1);
    return ret;
#endif
}


/******************************************************************************/
