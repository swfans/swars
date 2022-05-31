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
  int WeaponsCarried;
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
    short ThingOffset;
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

#pragma pack()
/******************************************************************************/
void init_things(void);
TbResult delete_node(struct Thing *p_thing);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
