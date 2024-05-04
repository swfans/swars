/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file vehtraffic.c
 *     Pathfinding triangulation support required by the 3D engine.
 * @par Purpose:
 *     Implement functions for handling pathfinding triangulation.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     29 Sep 2023 - 02 Jan 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "vehtraffic.h"

#include <stdlib.h>
#include "bfutility.h"
#include "bigmap.h"
#include "game.h"
#include "game_speed.h"
#include "thing.h"
#include "vehicle.h"
#include "swlog.h"
/******************************************************************************/
extern uint dword_1C8470;
extern int dword_1C8474[32];

short tnode_find_used_link(struct TrafficNode *p_tnode)
{
    short i;
    for (i = 0; i < 4; i++)
    {
        if (p_tnode->UTraffic.Link[i] != 0)
            return i;
    }
    return -1;
}

short get_next_tnode(struct Thing *p_vehicle, struct TrafficNode *p_tnode)
{
    short ret;
    asm volatile (
      "call ASM_get_next_tnode\n"
        : "=r" (ret) : "a" (p_vehicle), "d" (p_tnode));
    return ret;
}

void process_next_vnav_turn(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_next_vnav_turn\n"
        : : "a" (p_vehicle));
    return;
}

void remove_locks(struct Thing *p_thing)
{
    asm volatile ("call ASM_remove_locks\n"
        : : "a" (p_thing));
    return;
}

int check_person_close(struct TrafficNode *p_tnode)
{
    int ret;
    asm volatile (
      "call ASM_check_person_close\n"
        : "=r" (ret) : "a" (p_tnode));
    return ret;
}

void remove_locks_apart_from_agok(struct Thing *p_thing)
{
    asm volatile ("call ASM_remove_locks_apart_from_agok\n"
        : : "a" (p_thing));
    return;
}

void update_vehicle_elevation(struct Thing *p_vehicle, short statn)
{
    short angleX;

    angleX = 0;
    if (statn > 0)
    {
        struct Thing *p_station;
        short nxstatn;

        p_station = &things[statn];
        if ((p_station->Flag & 0x0002) != 0)
        {
            LOGERR("Crashing vehicle %d due to destroyed station %d",
              p_vehicle->ThingOffset, statn);
            start_crashing(p_vehicle);
            return;
        }
        if ((p_vehicle->Flag & 0x8000000) != 0)
            nxstatn = p_station->U.UObject.NextThing;
        else
            nxstatn = p_station->U.UObject.PrevThing;

        if (nxstatn > 0)
        {
            int cux, cuy, cuz, nxx, nxy, nxz, dy, faxz;
            struct Thing *p_nxstation;
            int dx, dz;

            p_nxstation = &things[nxstatn];
            cux = (p_station->X >> 8) - 5 * p_station->U.UObject.OffX;
            nxx = (p_nxstation->X >> 8) - 5 * p_nxstation->U.UObject.OffX;
            cuz = (p_station->Z >> 8) - 5 * p_station->U.UObject.OffZ;
            nxz = (p_nxstation->Z >> 8) - 5 * p_nxstation->U.UObject.OffZ;
            cuy = (p_station->Y >> 3) + 3840;
            nxy = (p_nxstation->Y >> 3) + 3840;

            dx = abs(cux - nxx);
            dy = (cuy - nxy) >> 5;
            dz = abs(cuz - nxz);
            if (dx >= dz)
                faxz = dx + (dz >> 2) + (dz >> 3) - (dx >> 5) + (dz >> 6) - (dx >> 7) + (dz >> 7);
            else
                faxz = dz + (dx >> 2) + (dx >> 3) - (dz >> 5) + (dx >> 6) + (dx >> 7) - (dz >> 7);
            angleX = arctan(abs(dy), faxz) - 1024;
            if (dy < 0)
                angleX = -angleX;
            p_vehicle->Y += (cuy - p_vehicle->Y) >> 3;
        }
    }

    if ((p_vehicle->State == VehSt_GOTO_LOC) && ((gameturn & 3) == 0) && ((p_vehicle->U.UVehicle.Dummy4a & 0x3F) > 2))
    {
        int dx, dz;

        dx = abs(p_vehicle->U.UVehicle.destx - (p_vehicle->X >> 8));
        dz = abs(p_vehicle->U.UVehicle.destz - (p_vehicle->Z >> 8));
        if (dx <= dz)
            dx >>= 1;
        else
            dz >>= 1;
        if (dx + dz < 768)
            p_vehicle->State = VehSt_UNKN_3C;
    }
    if (p_vehicle->U.UVehicle.AngleX < angleX)
        p_vehicle->U.UVehicle.AngleX += 4;
    if (p_vehicle->U.UVehicle.AngleX > angleX)
        p_vehicle->U.UVehicle.AngleX -= 4;
}

void update_vehicle_speed_and_direction(struct Thing *p_vehicle, int tndt_x, int tndt_z, long nxdist_sq)
{
    short angleY, angDY;

    angleY = p_vehicle->U.UVehicle.AngleY - arctan(tndt_x, tndt_z);
    if (angleY < -1024)
        angleY += 2048;
    else if (angleY > 1024)
        angleY -= 2048;

    if (angleY > 0)
    {
        if (angleY >= 50)
          angDY = (2048 - (p_vehicle->Speed >> 3)) >> 2;
        else
          angDY = 2 * angleY;
        angDY = -angDY;
    }
    else
    {
        if (angleY <= -50)
            angDY = (2048 - (p_vehicle->Speed >> 3)) >> 2;
        else
            angDY = -2 * angleY;
    }
    p_vehicle->U.UVehicle.AngleDY = angDY;

    if ((angleY > 512) || (angleY < -512))
        angleY = 512;
    if (p_vehicle->SubState == 2)
    {
        p_vehicle->U.UVehicle.ReqdSpeed = nxdist_sq >= 0x8000 ? 2048 - 4 * abs(angleY) : 512;
        if (p_vehicle->U.UVehicle.ReqdSpeed < 10)
            p_vehicle->U.UVehicle.ReqdSpeed = 10;
    }
}

void process_vehicle_stop_for_pedestrians(struct Thing *p_vehicle)
{
    struct TrafficNode *p_tnode;
    struct Thing *p_gate;

    // Only Traffic Nodes support stopping for pedestrians
    if (p_vehicle->U.UVehicle.TNode >= 0)
        return;

    p_tnode = &game_traffic_nodes[-p_vehicle->U.UVehicle.TNode];
    if ((p_vehicle->U.UVehicle.WorkPlace & 0x0008) != 0)
    {
        if (p_vehicle->State == VehSt_GOTO_LOC || p_vehicle->State == VehSt_WANDER)
        {
            if ((p_tnode->Flags & 0x0040) != 0)
            {
                p_vehicle->U.UVehicle.ReqdSpeed = 0;
                p_gate = &things[p_tnode->GateLink];
                if ((p_gate->U.UObject.Timer[1] & 0x0008) != 0)
                {
                    struct TrafficNode *p_tztnode;

                    p_tztnode = &game_traffic_nodes[-p_gate->U.UObject.TargetDZ];
                    if ((p_tztnode->Flags & 0x0040) != 0)
                    {
                        if (p_tztnode->GateLink == p_vehicle->ThingOffset)
                        {
                            if (p_vehicle->U.UVehicle.Dummy4b == p_gate->U.UObject.TargetDZ)
                            {
                                p_tztnode->GateLink = 0;
                                p_tztnode->Flags &= ~0x0040;
                                p_vehicle->U.UVehicle.WorkPlace &= ~0x0020;
                            }
                            else
                            {
                                p_tztnode->GateLink = 0;
                                p_tztnode->Flags &= 0x0040;
                                p_vehicle->U.UVehicle.WorkPlace &= ~0x0040;
                            }
                            if (p_gate->U.UObject.GateBH.ValB == p_vehicle->U.UVehicle.TNode)
                            {
                                p_tztnode->GateLink = 0;
                                p_tztnode->Flags &= ~0x0040;
                                p_gate->U.UObject.Timer[1] &= ~0x0020;
                            }
                            else
                            {
                                struct TrafficNode *p_tnode;
                                p_tnode = &game_traffic_nodes[-p_vehicle->U.UVehicle.TNode];
                                p_tnode->GateLink = 0;
                                p_tnode->Flags &= ~0x0040;
                                p_gate->U.UObject.Timer[1] &= ~0x0040;
                            }
                        }
                    }
                }
            }
            else
            {
                remove_locks_apart_from_agok(p_vehicle);
                p_tnode->Flags |= 0x0040;
                p_tnode->GateLink = p_vehicle->ThingOffset;
                p_vehicle->U.UVehicle.WorkPlace |= 0x0010;
                p_vehicle->U.UVehicle.WorkPlace &= ~0x0008;
            }
        }
    }

    if (((p_tnode->Flags & 0x0008) != 0) && check_person_close(p_tnode))
    {
        if (p_vehicle->U.UVehicle.PissedOffWithWaiting < 100)
        {
            if (p_vehicle->U.UVehicle.PissedOffWithWaiting == 0) {
                p_vehicle->U.UVehicle.ZebraOldHealth = p_vehicle->Health;
            }
            if (p_vehicle->U.UVehicle.ZebraOldHealth == p_vehicle->Health) {
                p_vehicle->U.UVehicle.ReqdSpeed = 0;
                p_vehicle->U.UVehicle.ZebraOldHealth = p_vehicle->Health;
                p_vehicle->U.UVehicle.PissedOffWithWaiting++;
            } else {
                p_vehicle->U.UVehicle.PissedOffWithWaiting = 200;
            }
        }
    }
    else
    {
        p_vehicle->U.UVehicle.PissedOffWithWaiting = 0;
    }
}

void vehicle_workplace_states_finish(struct Thing *p_vehicle)
{
    if ((p_vehicle->U.UVehicle.WorkPlace & 0x0020) != 0)
    {
        struct TrafficNode *p_dmtnode;
        int dx, dz;

        p_dmtnode = &game_traffic_nodes[-p_vehicle->U.UVehicle.Dummy4b];
        dx = (p_vehicle->X >> 8) - p_dmtnode->X;
        dz = (p_vehicle->Z >> 8) - p_dmtnode->Z;
        if (dz * dz + dx * dx > 0x20000)
        {
            p_dmtnode->GateLink = 0;
            p_dmtnode->Flags &= ~0x0040;
            p_vehicle->U.UVehicle.WorkPlace &= ~0x0020;
        }
    }

    if ((p_vehicle->U.UVehicle.WorkPlace & 0x0040) != 0)
    {
        struct TrafficNode *p_agtnode;
        int dx, dz;

        p_agtnode = &game_traffic_nodes[-p_vehicle->U.UVehicle.Agok];
        dx = (p_vehicle->X >> 8) - p_agtnode->X;
        dz = (p_vehicle->Z >> 8) - p_agtnode->Z;
        if (dz * dz + dx * dx > 0x20000)
        {
            p_agtnode->GateLink = 0;
            p_agtnode->Flags &= ~0x0040;
            p_vehicle->U.UVehicle.WorkPlace &= ~0x0040;
        }
    }
}

void process_next_tnode(struct Thing *p_vehicle)
{
#if 0
    asm volatile ("call ASM_process_next_tnode\n"
        : : "a" (p_vehicle));
    return;
#endif
    short tnode;
    int tndt_x, tndt_z;
    int nxdist_sq; //< Squared distance to next traffic node or station

    tnode = p_vehicle->U.UVehicle.TNode;
    if (tnode == 0) {
        LOGERR("Crashing vehicle %d due to no TNode assigned", p_vehicle->ThingOffset);
        start_crashing(p_vehicle);
        return;
    }

    if ((p_vehicle->State == VehSt_UNKN_34 || p_vehicle->State == VehSt_UNKN_33)
      && (tnode == 0) && (p_vehicle->Speed == 0))
    {
        p_vehicle->State = VehSt_PARKED_PARAL;
        p_vehicle->U.UVehicle.AccelZ = 0;
        p_vehicle->U.UVehicle.AccelX = 0;
        p_vehicle->Flag |= 0x0004;
        p_vehicle->U.UVehicle.WorkPlace &= ~0x0100;
    }

    {
        int cux, cuz;

        if (tnode <= 0) // we've alteady made sure it's not 0
        {
            struct TrafficNode *p_tnode;

            p_tnode = &game_traffic_nodes[-tnode];
            if (p_vehicle->State == VehSt_UNKN_33) {
                cux = p_vehicle->U.UVehicle.Dummy5b;
                cuz = p_vehicle->U.UVehicle.Dummy5a;
            } else {
                cux = p_tnode->X;
                cuz = p_tnode->Z;
            }
        }
        else
        {
            struct Thing *p_station;

            p_station = &things[tnode];
            if ((p_vehicle->Flag & 0x8000000) != 0) {
                cux = PRCCOORD_TO_MAPCOORD(p_station->X) - 5 * p_station->U.UObject.OffX;
                cuz = PRCCOORD_TO_MAPCOORD(p_station->Z) - 5 * p_station->U.UObject.OffZ;
            } else {
                cux = PRCCOORD_TO_MAPCOORD(p_station->X) + 5 * p_station->U.UObject.OffX;
                cuz = PRCCOORD_TO_MAPCOORD(p_station->Z) + 5 * p_station->U.UObject.OffZ;
            }
        }

        tndt_x = cux - PRCCOORD_TO_MAPCOORD(p_vehicle->X);
        tndt_z = cuz - PRCCOORD_TO_MAPCOORD(p_vehicle->Z);
        nxdist_sq = tndt_z * tndt_z + tndt_x * tndt_x;
    }

    if (tnode > 0)
    {
        p_vehicle->U.UVehicle.ReqdSpeed = 1512;
    }
    else if (tnode < 0)
    {
        struct TrafficNode *p_tnode;

        p_tnode = &game_traffic_nodes[-tnode];
        p_vehicle->U.UVehicle.ReqdSpeed = 1200 - 2 * abs(p_vehicle->U.UVehicle.AngleDY);
        if ((p_vehicle->U.UVehicle.WorkPlace & 0x0008)
         && (p_vehicle->U.UVehicle.WorkPlace & 0x0010)
         && (p_tnode->GateLink == p_vehicle->ThingOffset)) {
            p_vehicle->U.UVehicle.WorkPlace &= ~0x0008;
        }
        if (((p_vehicle->U.UVehicle.WorkPlace & 0x0008) != 0)
         && ((p_tnode->Flags & (0x0010|0x0020)) == 0)) {
            p_vehicle->U.UVehicle.WorkPlace &= ~0x0008;
        }
        if (((p_vehicle->U.UVehicle.WorkPlace & 0x0010) != 0)
         && ((p_tnode->Flags & (0x0010|0x0020)) == 0)) {
            p_vehicle->U.UVehicle.WorkPlace &= ~0x0010;
            p_tnode->Flags &= ~0x0040;
            p_tnode->GateLink = 0;
        }
    }

    if (tnode < 0)
    {
        struct TrafficNode *p_tnode;
        struct Thing *p_gate;

        p_tnode = &game_traffic_nodes[-tnode];
        if ((p_tnode->GateLink != 0) && ((p_tnode->Flags & 0x8000) != 0) && (nxdist_sq < 0x90000))
        {
            p_gate = &things[p_tnode->GateLink];
            if ((p_gate->Flag & 0x0040) == 0) {
                p_gate->Flag |= 0x0020;
                p_vehicle->U.UVehicle.ReqdSpeed = 0;
                p_vehicle->U.UVehicle.AngleDY = 0;
                return;
            }
        }
    }

    if (tnode < 0)
    {
        struct TrafficNode *p_tnode;
        struct Thing *p_gate;

        p_tnode = &game_traffic_nodes[-tnode];
        if ((p_tnode->GateLink != 0) && ((p_tnode->Flags & 0x8000) == 0) && (nxdist_sq < 0x10000))
        {
            p_gate = &things[p_tnode->GateLink];
            p_gate->Flag |= 0x0100;
        }
    }

    if (tnode > 0)
    {
        struct Thing *p_station;
        short nxstatn;

        if (nxdist_sq < 0xC000)
        {
            p_station = &things[tnode];
            if ((p_vehicle->Flag & 0x8000000) != 0)
                nxstatn = p_station->U.UObject.PrevThing;
            else
                nxstatn = p_station->U.UObject.NextThing;
            p_vehicle->U.UVehicle.TNode = nxstatn;
        }
    }

    TbBool do_state = false;
    if (tnode < 0)
    {
        struct TrafficNode *p_tnode;

        p_tnode = &game_traffic_nodes[-tnode];
        if ((p_tnode->GateLink != 0) && (nxdist_sq < 0x10000))
            do_state = true;
        if ((p_tnode->GateLink == 0) && (nxdist_sq < 0x20000))
            do_state = true;
    }

    if (do_state)
    {
        struct TrafficNode *p_tnode;
        struct TrafficNode *p_nxtnode;

        p_tnode = &game_traffic_nodes[-tnode];
        switch (p_vehicle->State)
        {
        case VehSt_GOTO_LOC:
            if ((p_tnode->Flags & (0x0010|0x0020)) != 0)
                p_vehicle->U.UVehicle.WorkPlace |= 0x0001;
            else
                p_vehicle->U.UVehicle.WorkPlace &= ~0x0001;
            if ((p_vehicle->U.UVehicle.WorkPlace & 0x0020) != 0)
            {
                if ((p_vehicle->U.UVehicle.WorkPlace & 0x0040) != 0) {
                    struct TrafficNode *p_agtnode;

                    p_agtnode = &game_traffic_nodes[-p_vehicle->U.UVehicle.Agok];
                    p_agtnode->Flags &= ~0x0040;
                    p_agtnode->GateLink = 0;
                    p_vehicle->U.UVehicle.WorkPlace &= ~0x0040;
                }
                p_vehicle->U.UVehicle.Agok = p_vehicle->U.UVehicle.Dummy4b;
                p_vehicle->U.UVehicle.WorkPlace &= ~(0x0020|0x0040);
                p_vehicle->U.UVehicle.WorkPlace |= 0x0040;
            }
            if ((p_vehicle->U.UVehicle.WorkPlace & 0x0010) != 0) {
                p_vehicle->U.UVehicle.WorkPlace |= 0x0020;
                p_vehicle->U.UVehicle.WorkPlace &= ~0x0010;
            }
            process_next_vnav_turn(p_vehicle);
            if (p_vehicle->U.UVehicle.TNode < 0)
            {
                p_nxtnode = &game_traffic_nodes[-p_vehicle->U.UVehicle.TNode];
                if ((p_nxtnode->Flags & 0x30) != 0) {
                    nxdist_sq = 0x7FFFFFFF;
                    p_vehicle->U.UVehicle.WorkPlace |= 0x0008;
                }
            }
            break;
        case VehSt_UNKN_33:
            p_tnode = &game_traffic_nodes[-p_vehicle->U.UVehicle.TNode];
            p_nxtnode = &game_traffic_nodes[-p_vehicle->U.UVehicle.Dummy4b];
            if ((p_tnode->Flags & 0x0800) != 0)
            {
                int dx, dz;

                dx = p_tnode->X - p_nxtnode->X;
                dz = p_tnode->Z - p_nxtnode->Z;
                p_vehicle->U.UVehicle.AccelZ = 0;
                p_vehicle->U.UVehicle.AccelX = 0;
                p_vehicle->State = VehSt_UNKN_36;
                p_vehicle->U.UVehicle.Dummy5b = arctan(dx, dz);
                p_vehicle->U.UVehicle.Dummy5a = 0;
            }
            else
            {
                int simp_dist;
                int accX, accZ;
                int selX, selZ;
                int dx, dz;

                dx = p_tnode->X - p_nxtnode->X;
                dz = p_nxtnode->Z - p_tnode->Z;
                if (abs(dx) >= abs(dz)) {
                    simp_dist = abs(dx) + (abs(dz) >> 1);
                } else {
                    simp_dist = abs(dz) + (abs(dx) >> 1);
                }
                accX = 32 * dz;
                accZ = 32 * dx;
                if (simp_dist > 0) {
                    accX /= simp_dist;
                    accZ /= simp_dist;
                }
                selX = (p_vehicle->X >> 8) + 4 * accX + 4 * accZ + 8 * accX;
                selZ = (p_vehicle->Z >> 8) - 4 * accX + 4 * accZ + 8 * accZ;
                if (((p_vehicle->U.UVehicle.WorkPlace & 0x0100) != 0)
                  || !check_for_a_vehicle_here(selX, selZ, p_vehicle))
                {
                    dx = p_tnode->X - p_nxtnode->X;
                    dz = p_tnode->Z - p_nxtnode->Z;
                    p_vehicle->State = VehSt_UNKN_36;
                    p_vehicle->U.UVehicle.Dummy5a = 0;
                    p_vehicle->U.UVehicle.Dummy5b = arctan(dx, dz);
                    p_vehicle->U.UVehicle.AccelZ = accZ;
                    p_vehicle->U.UVehicle.AccelX = accX;
                    p_vehicle->U.UVehicle.Dummy5a |= 0x11;
                }
                else
                {
                    p_vehicle->U.UVehicle.Dummy5a -= 32 * accX;
                    p_vehicle->U.UVehicle.Dummy5b += 32 * accZ;
                    p_vehicle->U.UVehicle.WorkPlace |= 0x0100;
                }
            }
            break;
        case VehSt_UNKN_34:
            tnode = p_vehicle->U.UVehicle.TNode;
            p_vehicle->U.UVehicle.Dummy4b = 0;
            p_vehicle->U.UVehicle.Dummy4a = tnode;
            p_vehicle->State = VehSt_UNKN_36;
            p_tnode = &game_traffic_nodes[-tnode];
            p_vehicle->U.UVehicle.TNode = p_tnode->UTraffic.Link[0];
            break;
        default:
            p_vehicle->U.UVehicle.WorkPlace &= ~0x0004;
            if ((p_tnode->Flags & 0x0080) == 0)
            {
                short lnk;

                if ((p_tnode->Flags & 0x0030) != 0)
                    p_vehicle->U.UVehicle.WorkPlace |= 0x0001;
                else
                    p_vehicle->U.UVehicle.WorkPlace &= ~0x0001;

                if ((p_vehicle->U.UVehicle.WorkPlace & 0x0020) != 0)
                {
                    if ((p_vehicle->U.UVehicle.WorkPlace & 0x0040) != 0)
                    {
                        struct TrafficNode *p_agtnode;

                        p_agtnode = &game_traffic_nodes[-p_vehicle->U.UVehicle.Agok];
                        p_agtnode->Flags &= ~0x0040;
                        p_agtnode->GateLink = 0;
                        p_vehicle->U.UVehicle.WorkPlace &= ~0x0040;
                    }
                    p_vehicle->U.UVehicle.Agok = p_vehicle->U.UVehicle.Dummy4b;
                    p_vehicle->U.UVehicle.WorkPlace &= ~(0x0020|0040);
                    p_vehicle->U.UVehicle.WorkPlace |= 0x0040;
                }

                p_vehicle->U.UVehicle.Dummy4a = 0;
                p_vehicle->U.UVehicle.Dummy4b = p_vehicle->U.UVehicle.TNode;
                if ((p_vehicle->U.UVehicle.WorkPlace & 0x0010) != 0)
                {
                    p_vehicle->U.UVehicle.WorkPlace |= 0x0020;
                    p_vehicle->U.UVehicle.WorkPlace &= ~0x0010;
                }
                lnk = get_next_tnode(p_vehicle, p_tnode);
                p_vehicle->U.UVehicle.TNode = p_tnode->UTraffic.Link[lnk];
                if (p_vehicle->U.UVehicle.TNode == 0) {
                    LOGERR("Crashing vehicle %d state %d due to no next TNode in TNode %d link %d",
                      p_vehicle->ThingOffset, p_vehicle->State, p_vehicle->U.UVehicle.Dummy4b, lnk);
                    start_crashing(p_vehicle);
                }

                if (p_vehicle->U.UVehicle.TNode < 0)
                {
                    struct TrafficNode *p_lntnode;

                    p_lntnode = &game_traffic_nodes[-p_vehicle->U.UVehicle.TNode];
                    if (((p_tnode->UTraffic.Flags[lnk] & 0x80) != 0) ||
                      ((p_lntnode->Flags & (0x0010|0x0020)) != 0))
                    {
                        nxdist_sq = 0x7FFFFFFF;
                        p_vehicle->U.UVehicle.WorkPlace |= 0x0008;
                    }
                }
                if ((p_tnode->UTraffic.Flags[lnk] & 0x01) != 0)
                    p_vehicle->Flag |= 0x0800;
                else
                    p_vehicle->Flag &= ~0x0800;
            }
            else
            {
                int i, lnk;
                int dx, dz;

                for (i = 0; i < 8; i++)
                {
                    struct TrafficNode *p_lntnode;

                    p_vehicle->U.UVehicle.TNode = -dword_1C8474[LbRandomAnyShort() % dword_1C8470];
                    p_tnode = &game_traffic_nodes[-p_vehicle->U.UVehicle.TNode];

                    lnk = tnode_find_used_link(p_tnode);
                    if (lnk < 0)
                        break;

                    p_lntnode = &game_traffic_nodes[-p_tnode->UTraffic.Link[lnk]];
                    if ((p_lntnode->Flags & 0x0040) == 0)
                    {
                        struct TrafficNode *p_agtnode;

                        dx = (p_lntnode->X << 8) - (p_tnode->X << 8);
                        dz = (p_lntnode->Z << 8) - (p_tnode->Z << 8);
                        p_tnode = &game_traffic_nodes[-p_vehicle->U.UVehicle.TNode];
                        move_mapwho(p_vehicle, p_tnode->X << 8, (p_tnode->Y + 15) << 8, p_tnode->Z << 8);
                        remove_locks(p_vehicle);
                        p_vehicle->U.UVehicle.WorkPlace |= 0x0040;
                        p_vehicle->U.UVehicle.Agok = p_tnode->UTraffic.Link[lnk];
                        p_agtnode = &game_traffic_nodes[-p_vehicle->U.UVehicle.Agok];
                        p_agtnode->Flags |= 0x0040;
                        p_agtnode->GateLink = p_vehicle->ThingOffset;
                        break;
                    }
                }
                if (i >= 8) {
                    LOGERR("Crashing vehicle %d state %d due to no random next TNode despite %d tries",
                      p_vehicle->ThingOffset, p_vehicle->State, i);
                    start_crashing(p_vehicle);
                    return;
                }
                p_vehicle->U.UVehicle.AngleDY = 0;
                p_vehicle->U.UVehicle.AngleY = arctan(dx, dz);
            }
            break;
        }
    }

    update_vehicle_elevation(p_vehicle, tnode);

    update_vehicle_speed_and_direction(p_vehicle, tndt_x, tndt_z, nxdist_sq);

    if (nxdist_sq < 0xC0000)
    {
        process_vehicle_stop_for_pedestrians(p_vehicle);
    }

    vehicle_workplace_states_finish(p_vehicle);
}


/******************************************************************************/
