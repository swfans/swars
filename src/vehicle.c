/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file vehicle.c
 *     Routines implementing vehicle.
 * @par Purpose:
 *     Implement functions for handling vehicle things.
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
#include "vehicle.h"

#include "bfmath.h"
#include "bfutility.h"
#include "ssampply.h"
#include "bigmap.h"
#include "bmbang.h"
#include "game.h"
#include "pathtrig.h"
#include "sound.h"
#include "thing.h"
#include "swlog.h"
/******************************************************************************/

#pragma pack(1)

struct CarGlare { // sizeof=7
    short Dix;
    short Diy;
    short Diz;
    ubyte Flag;
};

#pragma pack()

struct CarGlare car_glare[] = {
  {0, 16, 336, 0},
  {-96, -64, 400, 0},
  {128, -64, 400, 0},
  {0, -96, 304, 0},
  {-208, -32, -240, 0},
  {208, -32, -240, 0},
  {128, -144, 240, 0},
  {-112, -144, 240, 0},
  {96, -32, 304, 0},
  {-80, -32, 304, 0},
  {96, -96, 528, 0},
  {-80, -96, 528, 0},
  {-128, -16, 288, 1},
  {144, -16, 288, 2},
  {112, -112, -96, 2},
  {-96, -112, -96, 1},
  {-80, -368, 48, 1},
  {96, -368, 48, 2},
  {-32, -64, 160, 0},
  {112, -16, 304, 0},
  {-96, -16, 304, 0},
  {-96, -224, 224, 1},
  {80, -224, 224, 2},
  {-96, -224, -288, 2},
  {80, -224, -288, 1},
  {-144, 0, 224, 1},
  {176, 0, 224, 2},
  {-112, -32, 448, 0},
  {144, -32, 448, 0},
  {-224, -176, 576, 0},
  {224, -176, 576, 0},
  {0, 16, 336, 0},
  {-96, -64, 400, 0},
  {128, -64, 400, 0},
  {-160, -672, -688, 0},
  {160, -672, -688, 0},
  {-112, -32, 448, 0},
  {144, -32, 448, 0},
  {0, -96, 304, 0},
  {112, -16, 304, 0},
  {-96, -16, 304, 0},
};

void init_mech(void)
{
    asm volatile ("call ASM_init_mech\n"
        :  :  : "eax" );
}

void mech_unkn_func_02(void)
{
    asm volatile ("call ASM_mech_unkn_func_02\n"
        :  :  : "eax" );
}

void mech_unkn_func_09(short thing)
{
    asm volatile ("call ASM_mech_unkn_func_09\n"
        : : "a" (thing));
}

void veh_add(struct Thing *p_thing, short frame)
{
    asm volatile ("call ASM_veh_add\n"
        : : "a" (p_thing), "d" (frame));
}

ushort veh_passenger_count(struct Thing *p_veh)
{
    struct Thing *p_thing;
    short thing;
    ushort c;

    c = 0;

    thing = p_veh->U.UVehicle.PassengerHead;
    while (thing > 0 && c < THINGS_LIMIT)
    {
        c++;
        p_thing = &things[thing];
        thing = p_thing->U.UPerson.LinkPassenger;
    }
    return c;
}

void VNAV_unkn_func_207(struct Thing *p_thing)
{
    asm volatile ("call ASM_VNAV_unkn_func_207\n"
        : : "a" (p_thing));
}

void start_crashing(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_start_crashing\n"
        : : "a" (p_vehicle));
}

void process_shuttle_pod(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_shuttle_pod\n"
        : : "a" (p_vehicle));
}

void init_vehicle_explode(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_init_vehicle_explode\n"
        : : "a" (p_vehicle));
}

void process_tank_stationary(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_tank_stationary\n"
        : : "a" (p_vehicle));
}

void process_tank(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_tank\n"
        : : "a" (p_vehicle));
}

void set_passengers_location(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_set_passengers_location\n"
        : : "a" (p_vehicle));
}

short angle_between_points(int x1, int z1, int x2, int z2)
{
  return LbArcTanAngle(x2 - x1, z1 - z2);
}

void process_tank_turret(struct Thing *p_tank)
{
    struct Thing *p_turret;
    int target_x, target_y;
    int turret;
    short angle;
    int dt_angle;

    turret = p_tank->U.UVehicle.SubThing;
    if (turret == 0) {
        p_tank->OldTarget = LbFPMath_PI+1;
        return;
    }
    p_turret = &things[turret];
    if ((p_tank->Flag & TngF_Unkn20000000) != 0)
    {
        target_x = p_tank->U.UVehicle.TargetDX;
        target_y = p_tank->U.UVehicle.TargetDZ;
    }
    else
    {
        struct Thing *p_target;
        p_target = p_tank->PTarget;
        if (p_target == NULL)
        {
            p_tank->OldTarget = 20000;
            return;
        }
        target_x = PRCCOORD_TO_MAPCOORD(p_target->X);
        target_y = PRCCOORD_TO_MAPCOORD(p_target->Z);
    }
    angle = p_turret->U.UMGun.AngleY
        - angle_between_points(target_x, target_y, PRCCOORD_TO_MAPCOORD(p_tank->X), PRCCOORD_TO_MAPCOORD(p_tank->Z));
    if (angle < -LbFPMath_PI)
        angle += 2*LbFPMath_PI;
    else if (angle > LbFPMath_PI)
        angle -= 2*LbFPMath_PI;

    //TODO CONFIG how fast the tank can target could be a difficulty-related setting
    // Travel 1/8 of the distance in each game turn
    dt_angle = angle / 8;
    if (dt_angle > LbFPMath_PI/17)
        dt_angle = LbFPMath_PI/17;
    if (dt_angle < -LbFPMath_PI/17)
        dt_angle = -LbFPMath_PI/17;
    if (dt_angle == 0)
    {
        if (angle > 0)
            dt_angle = 1;
        if (angle < 0)
            dt_angle = -1;
    }

    // Despite being caused by the turret, we bind the sound samples to the vehicle part
    // of the tank. This is because turrets do not contain full position on map, so the
    // sound update would misplace the sound source if it was bound to the turret
    if ((p_turret->Flag2 & 0x0200) != 0)
    {
        if (dt_angle <= 1) {
            // Play rotation stop sample
            if (!IsSamplePlaying(p_tank->ThingOffset, 47, 0))
                play_dist_sample(p_tank, 47, 127, 0x40u, 100, 0, 1);
            p_turret->Flag2 &= ~0x0200;
        }
    }
    else
    {
        // Play rotation sample if moving over 1.2 degree per turn and the angle is not getting smaller.
        // Huge values of OldTarget (beyond pi) indicate that previously we had no target.
        if ((abs(dt_angle) >= LbFPMath_PI/75) && (p_tank->OldTarget < abs(angle) || p_tank->OldTarget > LbFPMath_PI)) {
            if (!IsSamplePlaying(p_tank->ThingOffset, 48, 0))
                play_dist_sample(p_tank, 48, 127, 0x40u, 100, 0, 1);
            p_turret->Flag2 |= 0x0200;
        }
    }

    p_turret->U.UMGun.AngleY -= dt_angle;
    p_tank->OldTarget = abs(angle);
    p_turret->U.UMGun.AngleY = (p_turret->U.UMGun.AngleY + 2*LbFPMath_PI) & LbFPMath_AngleMask;
}

void process_ship(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_ship\n"
        : : "a" (p_vehicle));
}

void process_mech_stationary(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_mech_stationary\n"
        : : "a" (p_vehicle));
}

void process_mech(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_mech\n"
        : : "a" (p_vehicle));
}

void process_mech_unknown1(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_mech_unknown1\n"
        : : "a" (p_vehicle));
}

void process_vehicle(struct Thing *p_vehicle)
{
    asm volatile ("call ASM_process_vehicle\n"
        : : "a" (p_vehicle));
}


/******************************************************************************/
