/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file thing.h
 *     Header file for thing.c.
 * @par Purpose:
 *     Thing structure support.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef THING_H
#define THING_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

enum ThingType {
    TT_NONE = 0x0,
    TT_UNKN1 = 0x1,
    TT_VEHICLE = 0x2,
    TT_PERSON = 0x3,
    TT_UNKN4 = 0x4,
    SmTT_STATIC = 0x5,
    TT_ROCKET = 0x6,
    TT_UNKN7 = 0x7,
    TT_UNKN8 = 0x8,
    TT_BUILDING = 0x9,
    TT_UNKN10 = 0xA,
    TT_LASER11 = 0xB,
    TT_LASER_GUIDED = 0xC,
    TT_UNKN13 = 0xD,
    TT_UNKN14 = 0xE,
    TT_MINE = 0xF,
    TT_GRENADE = 0x10,
    SmTT_SPARK = 0x11,
    TT_UNKN18 = 0x12,
    TT_LASER_ELEC = 0x13,
    SmTT_INTELLIG_DOOR = 0x14,
    SmTT_SCALE_EFFECT = 0x15,
    SmTT_NUCLEAR_BOMB = 0x16,
    TT_UNKN23 = 0x17,
    SmTT_SMOKE_GENERATOR = 0x18,
    SmTT_MINE = 0x19,
    SmTT_CARRIED_ITEM = 0x1A,
    SmTT_ELECTRIC_STRAND = 0x1B,
    TT_RAZOR_WIRE = 0x1C,
    TT_LASER29 = 0x1D,
    SmTT_TIME_POD = 0x1E,
    TT_AIR_STRIKE = 0x1F,
    SmTT_CANISTER = 0x20,
    TT_UNKN33 = 0x21,
    TT_UNKN34 = 0x22,
    TT_UNKN35 = 0x23,
    SmTT_STASIS_POD = 0x24,
    SmTT_SOUL = 0x25,
    TT_LASER38 = 0x26,
    TT_UNKN39 = 0x27,
    TT_UNKN40 = 0x28,
    TT_UNKN41 = 0x29,
    TT_UNKN42 = 0x2A,
    TT_UNKN43 = 0x2B,
    TT_UNKN44 = 0x2C,
    TT_UNKN45 = 0x2D,
    TT_UNKN46 = 0x2E,
    TT_UNKN47 = 0x2F,
    TT_UNKN48 = 0x30,
    TT_UNKN49 = 0x31,
    SmTT_BANG = 0x32,
    TT_UNKN51 = 0x33,
    SmTT_FIRE = 0x34,
    SmTT_SFX = 0x35,
    SmTT_TEMP_LIGHT = 0x36,
    TT_UNKN55 = 0x37,
    TT_UNKN56 = 0x38,
};

enum SubTypePerson {
  SubTT_PERS_NONE = 0x0,
  SubTT_PERS_AGENT = 0x1,
  SubTT_PERS_ZEALOT = 0x2,
  SubTT_PERS_PUNK_F = 0x3,
  SubTT_PERS_BRIEFCASE_M = 0x4,
  SubTT_PERS_WHITE_BRUN_F = 0x5,
  SubTT_PERS_MERCENARY = 0x6,
  SubTT_PERS_MECH_SPIDER = 0x7,
  SubTT_PERS_POLICE = 0x8,
  SubTT_PERS_PUNK_M = 0x9,
  SubTT_PERS_SCIENTIST = 0xA,
  SubTT_PERS_SHADY_M = 0xB,
  SubTT_PERS_HIGH_PRIEST = 0xC,
  SubTT_PERS_WHIT_BLOND_F = 0xD,
  SubTT_PERS_LETH_JACKT_M = 0xE,
  SubTT_PERS_FAST_BLOND_F = 0xF,
};

/** State of Thing of type Object.
 */
struct TngUObject
{
    short NextThing;
    short PrevThing;
    ubyte Group;
    ubyte EffectiveGroup;
    short Object;
    short MatrixIndex;
    ubyte NumbObjects;
    ubyte Angle;
    ubyte Token;
    char TokenDir;
    char OffX;
    char OffZ;
    short TargetDX;
    short TargetDY;
    short TargetDZ;
    ushort BuildStartVect;
    ushort BuildNumbVect;
    ushort ZZ_unused_but_pads_to_long_ObjectNo;
    short ComHead;
    short ComCur;
    short Mood;
    short RaiseDY[2];
    short RaiseY[2];
    short MinY[2];
    short Timer[2];
    short MaxY[2];
    short TNode;
    short Cost;
    short Shite;
    int BHeight;
    ushort Turn;
    ushort TurnPadOnPS;
    short tnode[4];
    ubyte player_in_me;
};

/** State of Thing of type MGun.
 */
struct TngUMGun
{
    short PathIndex;
    ushort UniqueID;
    ubyte Group;
    ubyte EffectiveGroup;
    short Object;
    short MatrixIndex;
    ubyte NumbObjects;
    ubyte ShotTurn;
    short WeaponTurn;
    ushort ObjectNo;
    short PrevThing;
    short NextThing;
    ubyte Token;
    char TokenDir;
    short AngleDY;
    short AngleX;
    short AngleY;
    short AngleZ;
    short GotoX;
    short GotoY;
    short GotoZ;
    int Dummy11[5];
    short RecoilTimer;
    ushort MaxHealth;
    ubyte CurrentWeapon;
    ubyte Dummy12;
    ubyte Dummy13[6];
};

/** State of Thing of type Vehicle.
 */
struct TngUVehicle
{
  short PathIndex;
  ushort UniqueID;
  ubyte Group;
  ubyte EffectiveGroup;
  short Object;
  short MatrixIndex;
  ubyte NumbObjects;
  ubyte Dummy2;
  short WeaponTurn;
  short ReqdSpeed;
  ushort MaxSpeed;
  ushort PassengerHead;
  short TNode;
  short AngleDY;
  short AngleX;
  short AngleY;
  short AngleZ;
  short GotoX;
  short GotoY;
  short GotoZ;
  short VehicleAcceleration;
  short LeisurePlace;
  short TargetDX;
  short TargetDY;
  short TargetDZ;
  short OnFace;
  short WorkPlace;
  short ComHead;
  short ComCur;
  short Timer2;
  short RecoilTimer;
  ushort MaxHealth;
  int Dummy[3];
  ushort SubThing;
  short Agok;
  int WobbleZP;
  int WobbleZV;
  ubyte Armour;
  ubyte PissedOffWithWaiting;
  short ZebraOldHealth;
  ushort destx;
  ushort destz;
};

/** State of Thing of type Effect.
 */
struct TngUEffect
{
  ushort MaxSpeed;
  short ReqdSpeed;
  ubyte Group;
  ubyte EffectiveGroup;
  short Object;
  short WeaponTurn;
  ubyte NumbObjects;
  ubyte Angle;
  ushort PassengerHead;
  short TNode;
  short AngleDY;
  short AngleX;
  short AngleY;
  short AngleZ;
  short GotoX;
  short GotoY;
  short GotoZ;
  short VehicleAcceleration;
  short MatrixIndex;
  short LeisurePlace;
};

/** Person Modifications.
 */
union Mod
{
  short Mods;
};

/** Person Draw Sprite Frame.
 */
struct DrawFrameId
{
  ubyte Version[5];
};

/** State of Thing of type Person.
 */
struct TngUPerson
{
  short PathIndex;
  ushort UniqueID;
  ubyte Group;
  ubyte EffectiveGroup;
  ushort ComHead;
  ushort ComCur;
  char SpecialTimer;
  ubyte Angle;
  short WeaponTurn;
  ubyte Brightness;
  ubyte ComRange;
  ubyte BumpMode;
  ubyte BumpCount;
  short Vehicle;
  short LinkPassenger;
  ushort Within;
  ushort LastDist;
  short ComTimer;
  short Timer2;
  short StartTimer2;
  ubyte AnimMode;
  ubyte OldAnimMode;
  short OnFace;
  union Mod UMod;
  short Mood;
  struct DrawFrameId FrameId;
  ubyte Shadows[4];
  ubyte RecoilTimer;
  ushort MaxHealth;
  ubyte Flag3;
  ubyte OldSubType;
  short ShieldEnergy;
  ubyte ShieldGlowTimer;
  ubyte WeaponDir;
  ushort SpecialOwner;
  ushort WorkPlace;
  ushort LeisurePlace;
  short WeaponTimer;
  short Target2;
  short MaxShieldEnergy;
  short PersuadePower;
  short MaxEnergy;
  short Energy;
  ubyte RecoilDir;
  ubyte CurrentWeapon;
  short GotoX;
  short GotoZ;
  short TempWeapon;
  short Stamina;
  short MaxStamina;
  ulong WeaponsCarried;
};

/** Structure for storing State of any Thing.
 * Everything in game besides ground terrain, are Things.
 */
struct Thing
{
    short Parent;
    short Next;
    short LinkParent;
    short LinkChild;
    ubyte SubType;
    ubyte Type;
    short State;
    ulong Flag;
    short LinkSame;
    short LinkSameGroup;
    short Radius;
    ushort ThingOffset;
    long X;
    long Y;
    long Z;
    short Frame;
    ushort StartFrame;
    short Timer1;
    short StartTimer1;
    long VX;
    long VY;
    long VZ;
    short Speed;
    short Health;
    ushort Owner;
    char PathOffset;
    char SubState;
    long PTarget;
    long Flag2;
    short GotoThingIndex;
    short OldTarget;
    union {
        struct TngUObject UObject;
        struct TngUMGun UMGun;
        struct TngUVehicle UVehicle;
        struct TngUEffect UEffect;
        struct TngUPerson UPerson;
    } U;
};

struct PeepStat
{
    short MaxHealth;
    short MaxShield;
    short MaximumStamina;
    short MaxEnergy;
    short PersuadeReqd;
    short PersuadeWorth;
    short Speed;
    short dum2;
    short dum3;
    short dum4;
};

#pragma pack()
/******************************************************************************/
extern ushort things_used_head;
extern struct PeepStat peep_type_stats[];

void init_things(void);
TbResult delete_node(struct Thing *p_thing);
void add_node_thing(ushort new_thing);
void remove_thing(ushort tngno);

void do_weapon_quantities1(struct Thing *p_person);
void do_weapon_quantities_proper1(struct Thing *p_person);
void set_person_stats_type(struct Thing *p_person, ushort type);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
