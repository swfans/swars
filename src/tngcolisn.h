/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file tngcolisn.h
 *     Header file for tngcolisn.c.
 * @par Purpose:
 *     Thing collisions support.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     04 Sep 2024 - 11 Nov 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef TNGCOLISN_H
#define TNGCOLISN_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct Thing;

struct ColVect { // sizeof=14
  short X1;
  short Y1;
  short Z1;
  short X2;
  short Y2;
  short Z2;
  short Face;
};

/** Collision vectors list.
 *
 * Contains a list of references to boundary vectors used for stepping
 * between ground faces and object faces (buildings).
 */
struct ColVectList { // sizeof=6
  ushort Vect; /**< Index of the ColVect with geometry vector. */
  ushort NextColList; /**< Index of the next ColVectList entry in a chain list, top bit is passability. */
  short Object; /**< Index of a Thing containing the object whose geometry has that vector. */
};

#pragma pack()
/******************************************************************************/
extern struct ColVectList *game_col_vects_list;
extern ushort next_vects_list;
extern struct ColVect *game_col_vects;
extern ushort next_col_vect;

void set_dome_col(struct Thing *p_building, ubyte flag);

void draw_engine_unk3_last(short x, short z);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
