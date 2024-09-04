/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file building.c
 *     Buildings related functions.
 * @par Purpose:
 *     Implement functions for handling buildings.
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
#include "building.h"

#include "bfmath.h"
#include "bfmemory.h"
#include "bfutility.h"
#include "bigmap.h"
#include "bmbang.h"
#include "enginsngobjs.h"
#include "game_speed.h"
#include "game.h"
#include "matrix.h"
#include "sound.h"
#include "thing.h"
#include "vehtraffic.h"
#include "swlog.h"
/******************************************************************************/
ubyte dome_open_speed = 4;


struct Thing *create_building_thing(int x, int y, int z, ushort obj, ushort nobj, ushort a6)
{
    struct Thing *ret;
    asm volatile (
      "push %6\n"
      "push %5\n"
      "call ASM_create_building_thing\n"
        : "=r" (ret) : "a" (x), "d" (y), "b" (z), "c" (obj), "g" (nobj), "g" (a6));
    return ret;
}

void set_dome_col(struct Thing *p_building, ubyte flag)
{
    struct ColVectList *p_cvlist;
    ushort vl_beg, vl_end;
    ushort  vl;

    // The thing data contains properties with range of ColVectList
    // which can switch the passability
    vl_beg = p_building->U.UObject.BuildStartVect;
    vl_end = vl_beg + p_building->U.UObject.BuildNumbVect;
    if (flag)
    {
        for (vl = vl_beg; vl < vl_end; vl++)
        {
            p_cvlist = &game_col_vects_list[vl];
            p_cvlist->NextColList |= 0x8000;
        }
    }
    else
    {
        for (vl = vl_beg; vl < vl_end; vl++)
        {
            p_cvlist = &game_col_vects_list[vl];
            p_cvlist->NextColList &= ~0x8000;
        }
    }
}

void do_dome_rotate1(struct Thing *p_building)
{
#if 0
    asm volatile ("call ASM_do_dome_rotate1\n"
        : : "a" (p_building));
#endif
    ushort obj, i;
    ushort pt1_beg, pt1_num;

    obj = p_building->U.UObject.Object;
    pt1_beg = game_objects[obj].StartPoint;
    pt1_num = game_objects[obj].EndPoint - pt1_beg;
    for (i = obj + 1; i < obj + 8; i++)
    {
        int factrS, factrC;
        short angle;
        ushort pt2_beg;
        ushort dpt;

        angle = (i - obj) * p_building->SubState;
        pt2_beg = game_objects[i].StartPoint;
        factrC = lbSinTable[angle + 512];
        factrS = lbSinTable[angle];
        for (dpt = 0; dpt <= pt1_num; dpt++)
        {
            struct SinglePoint *p_point1;
            struct SinglePoint *p_point2;
            p_point1 = &game_object_points[pt1_beg + dpt];
            p_point2 = &game_object_points[pt2_beg + dpt];
            p_point2->Y = (factrC * p_point1->Y - factrS * p_point1->Z) >> 16;
            p_point2->Z = (factrC * p_point1->Z + factrS * p_point1->Y) >> 16;
        }
    }
}

void process_dome1(struct Thing *p_building)
{
#if 0
    asm volatile ("call ASM_process_dome1\n"
        : : "a" (p_building));
#endif
    uint timer0;

    switch (p_building->State)
    {
    case 1:
        timer0 = p_building->SubState - dome_open_speed;
        p_building->SubState = timer0;
        if (timer0 <= 127)
        {
            if ((timer0 >= 111u) && (timer0 < 111u + dome_open_speed)) {
                set_dome_col(p_building, 1);
            }
            do_dome_rotate1(p_building);
        }
        else
        {
            p_building->SubState = 0;
            p_building->Timer1 = 100;
            p_building->State = 7;
        }
        break;
    case 4:
        timer0 = p_building->SubState + dome_open_speed;
        p_building->SubState = timer0;
        if (timer0 <= 127)
        {
            if ((timer0 >= 112u) && (timer0 < 112u + dome_open_speed)) {
                set_dome_col(p_building, 0);
            }
            do_dome_rotate1(p_building);
        }
        else
        {
            p_building->SubState = 127;
            p_building->Timer1 = 100;
            p_building->State = 8;
        }
        break;
    case 7:
        if ((p_building->Flag & TngF_Unkn0080) != 0)
            p_building->State = 4;
        p_building->Flag &= ~(TngF_Unkn0080|TngF_Unkn0040);
        break;
    case 8:
        if ((p_building->Flag & TngF_Unkn0040) != 0)
            p_building->State = 1;
        p_building->Flag &= ~(TngF_Unkn0080|TngF_Unkn0040);
        break;
    default:
        p_building->SubState = 127;
        p_building->Timer1 = 100;
        p_building->State = 8;
        break;
    }
}

ushort explode_thing_building(short thing, short x, short y, short z)
{
    ushort ret;
    asm volatile (
      "call ASM_explode_thing_building\n"
        : "=r" (ret) : "a" (thing), "d" (x), "b" (y), "c" (z));
    return ret;
}

void collapse_building_process_tnodes(struct Thing *p_building)
{
    short tnode1, tnode2;
    int i;

    for (i = 0; i < 4; i++)
    {
        short tnode;
        tnode = p_building->U.UObject.tnode_50[i];
        if (tnode != 0) {
            struct TrafficNode *p_tnode;
            p_tnode = &game_traffic_nodes[tnode];
            p_tnode->Flags |= 0x3000;
        }
    }

    tnode1 = p_building->U.UObject.tnode_50[0];
    tnode2 = p_building->U.UObject.tnode_50[1];
    if ((tnode1 != 0) && (tnode2 != 0))
    {
        struct TrafficNode *p_tnode;
        p_tnode = &game_traffic_nodes[tnode1];
        for (i = 0; i < 4; i++)
        {
            if (p_tnode->UTraffic.Link[i] == -tnode2) {
                break;
            }
            if (p_tnode->UTraffic.Link[i] == 0) {
                p_tnode->UTraffic.Link[i] = -tnode2;
                p_tnode->UTraffic.Flags[i] = 0x40;
                break;
            }
        }
    }

    tnode1 = p_building->U.UObject.tnode_50[2];
    tnode2 = p_building->U.UObject.tnode_50[3];
    if ((tnode1 != 0) && (tnode2 != 0))
    {
        struct TrafficNode *p_tnode;
        p_tnode = &game_traffic_nodes[tnode1];
        for (i = 0; i < 4; i++)
        {
            if (p_tnode->UTraffic.Link[i] == -tnode2) {
                break;
            }
            if (p_tnode->UTraffic.Link[i] == 0) {
                p_tnode->UTraffic.Link[i] = -tnode2;
                p_tnode->UTraffic.Flags[i] = 0x40;
                break;
            }
        }
    }

    tnode1 = p_building->U.UObject.tnode_50[0];
    tnode2 = p_building->U.UObject.tnode_50[3];
    if ((tnode1 != 0) && (tnode2 != 0))
    {
        struct TrafficNode *p_tnode;
        p_tnode = &game_traffic_nodes[tnode1];
        for (i = 0; i < 4; i++)
        {
            if (p_tnode->UTraffic.Link[i] == -tnode2) {
                p_tnode->UTraffic.Link[i] = 0;
                break;
            }
        }
    }

    tnode1 = p_building->U.UObject.tnode_50[2];
    tnode2 = p_building->U.UObject.tnode_50[1];
    if ((tnode1 != 0) && (tnode2 != 0))
    {
        struct TrafficNode *p_tnode;
        p_tnode = &game_traffic_nodes[tnode1];
        for (i = 0; i < 4; i++)
        {
            if (p_tnode->UTraffic.Link[i] == -tnode2) {
                p_tnode->UTraffic.Link[i] = 0;
                break;
            }
        }
    }
}

void collapse_building_shuttle_loader(short x, short y, short z, struct Thing *p_building)
{
    struct Thing *p_thing;
    short thing;

    p_thing = p_building;
    thing = p_building->ThingOffset;
    while ( 1 )
    {
        short pvthing;
        pvthing = thing;
        thing = p_thing->U.UObject.NextThing;
        if (thing == 0)
            break;
        if (thing <= 0)
        {
            if (p_building->SubType == SubTT_BLD_15) {
                tnode_all_unlink_thing(pvthing);
            }
            break;
        }
        else
        {
            p_thing = &things[thing];
            if ((p_thing->Flag & TngF_Unkn0002) != 0)
                break;
            p_thing->Flag |= TngF_Unkn0002;
            explode_thing_building(thing, x, y, z);
            p_thing->Type = TT_UNKN55;
        }
    }

    p_thing = p_building;
    thing = p_building->ThingOffset;
    while ( 1 )
    {
        short nxthing;
        nxthing = thing;
        thing = p_thing->U.UObject.PrevThing;
        if (thing == 0)
            break;
        if (thing <= 0)
        {
            if (p_building->SubType == SubTT_BLD_15) {
                tnode_all_unlink_thing(nxthing);
            }
            break;
        }
        else
        {
            p_thing = &things[thing];
            if ((p_thing->Flag & TngF_Unkn0002) != 0)
                break;
            p_thing->Flag |= TngF_Unkn0002;
            explode_thing_building(thing, x, y, z);
            p_thing->Type = TT_UNKN55;
        }
    }
}

void collapse_building_station(struct Thing *p_building)
{
    short cntr_x, cntr_z;
    short x, z;

    cntr_x = p_building->X >> 16;
    cntr_z = p_building->Z >> 16;

    for (x = cntr_x - 8; x <= cntr_x + 8; x++)
    {
        if ((x < 0) || (x > 127))
            continue;
        for (z = cntr_z - 8; z <= cntr_z + 8; z++)
        {
            struct MyMapElement *p_mapel;
            short thing;

            if ((z < 0) || (z > 127))
                continue;

            p_mapel = &game_my_big_map[128 * z + x];
            thing = p_mapel->Child;
            while (thing != 0)
            {
                if (thing > 0)
                {
                    struct Thing *p_thing;
                    p_thing = &things[thing];
                    if ((p_thing->Type == TT_BUILDING)
                      && (p_thing->SubType == SubTT_BLD_1C)
                      && ((p_thing->Flag & TngF_Unkn0002) == 0))
                    {
                        collapse_building(x << 8, 0, z << 8, p_thing);
                        break; // do not expect more than one building on a tile
                    }
                    thing = p_thing->Next;
                }
                else
                {
                    struct SimpleThing *p_sthing;
                    p_sthing = &sthings[thing];
                    thing = p_sthing->Next;
                }
            }
        }
    }
}

void collapse_building(short x, short y, short z, struct Thing *p_building)
{
#if 0
    asm volatile (
      "call ASM_collapse_building\n"
        :  : "a" (x), "d" (y), "b" (z), "c" (p_thing));
    return;
#endif
    struct SimpleThing *p_sthing;
    short thing;
    int i;

    if (ingame.SoundThing != 0)
    {
        p_sthing = &sthings[ingame.SoundThing];
        p_sthing->X = p_building->X;
        p_sthing->Y = p_building->Y;
        p_sthing->Z = p_building->Z;
        p_sthing->State = 1;
    }
    else
    {
        play_dist_sample(p_building, 0x2Du, 0x7Fu, 0x40u, 100, 0, 3);
        p_sthing = create_sound_effect(p_building->X >> 8, p_building->Y >> 8, p_building->Z >> 8, 0x2Eu, 127, -1);
        if (p_sthing != NULL)
        {
            p_sthing->State = 1;
            ingame.SoundThing = p_sthing->ThingOffset;
        }
    }
    thing = p_building->ThingOffset;

    collapse_building_process_tnodes(p_building);

    switch (p_building->SubType)
    {
    case SubTT_BLD_SHUTLDR:
    case SubTT_BLD_1D:
    case SubTT_BLD_1C:
    case SubTT_BLD_15:
        collapse_building_shuttle_loader(x, y, z, p_building);
        break;
    case SubTT_BLD_STATION:
        collapse_building_station(p_building);
        break;
    }

    if ((p_building->Flag & TngF_Unkn0002) == 0)
    {
        struct SingleObject *p_sobj;

        p_sobj = &game_objects[p_building->U.UObject.Object];
        if (((p_sobj->field_1C[1] & 0x01) == 0) || current_map == 9)// // map009 Singapore on-water map
        {
            quick_crater(p_building->X >> 16, p_building->Z >> 16, 3);
            for (i = 0; i < 32; i++)
            {
                int dx, dz;
                int bang_x, bang_z;
                dz = (LbRandomAnyShort() & 0xFF) - 127;
                dx = (LbRandomAnyShort() & 0xFF) - 127;
                bang_z = ((z + dz) << 8);
                bang_x = ((x + dx) << 8);
                bang_new4(bang_x, 0, bang_z, 65);
            }
        }
        p_building->Flag |= TngF_Unkn0002;
        explode_thing_building(thing, x, y, z);
        p_building->Type = TT_UNKN55;
    }
}

short create_a_pod(struct Thing *p_thing, struct Thing *p_loader, ushort pod_id)
{
    short ret;
    asm volatile (
      "call ASM_create_a_pod\n"
        : "=r" (ret) : "a" (p_thing), "d" (p_loader), "b" (pod_id));
    return ret;
}

void process_shuttle_loader(struct Thing *p_building)
{
    asm volatile ("call ASM_process_shuttle_loader\n"
        : : "a" (p_building));
}

void process_mounted_gun(struct Thing *p_building)
{
    asm volatile ("call ASM_process_mounted_gun\n"
        : : "a" (p_building));
}

void process_gate1(struct Thing *p_building)
{
    asm volatile ("call ASM_process_gate1\n"
        : : "a" (p_building));
}

void process_bld36(struct Thing *p_building)
{
    struct M33 *mat;
    mat = &local_mats[p_building->U.UObject.MatrixIndex];
    rotate_object_axis(mat, 0, 0, 32);
}

void process_building(struct Thing *p_building)
{
#if 0
    asm volatile ("call ASM_process_building\n"
        : : "a" (p_building));
#endif
    if ((p_building->Flag & TngF_Unkn00040000) != 0)
    {
        if ((p_building->Flag & TngF_Unkn0002) != 0)
        {
            p_building->Flag &= ~TngF_Unkn00040000;
            return;
        }
        collapse_building(p_building->X >> 8, p_building->Y >> 8, p_building->Z >> 8, p_building);
        return;
    }

    switch (p_building->SubType)
    {
    case SubTT_BLD_SHUTLDR:
        if ((p_building->Flag & TngF_Unkn0002) == 0)
        {
            if ((p_building->U.UObject.PrevThing == 0) && ((p_building->Flag & TngF_Unkn08000000) == 0)) {
                process_shuttle_loader(p_building);
            }
            if ((p_building->U.UObject.PrevThing == 0) && ((p_building->Flag & TngF_Unkn08000000) != 0)
              && ((LbRandomAnyShort() & 3) == 0) && (((gameturn + p_building->ThingOffset) & 7) == 0)) {
                create_a_pod(0, p_building, 0);
            }
        }
        break;
    case SubTT_BLD_DOME:
        if ((p_building->Flag & TngF_Unkn0002) == 0)
            process_dome1(p_building);
        break;
    case SubTT_BLD_MGUN:
        if ((p_building->Flag & TngF_Unkn0002) == 0)
            process_mounted_gun(p_building);
        break;
    case SubTT_BLD_GATE:
        process_gate1(p_building);
        break;
    case SubTT_BLD_36:
        process_bld36(p_building);
        break;
    default:
        if (p_building->State == 9) {
            collapse_building(p_building->X >> 8, p_building->Y >> 8, p_building->Z >> 8, p_building);
        }
        break;
    }
}

/******************************************************************************/
