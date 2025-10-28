/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file matrix.h
 *     Header file for matrix.c.
 * @par Purpose:
 *     Routines implementing matrix support.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef MATRIX_H
#define MATRIX_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct M33 {
  int R[3][3];
};

struct M31 {
  int R[3];
};

#pragma pack()
/******************************************************************************/
extern struct M33 local_mats[100];

void matrix_identity_fill(struct M33 *p_mat);
void matrix_transform(struct M31 *p_result, struct M33 *p_trans, struct M31 *p_source);
void rotate_object_axis(struct M33 *p_base, short xangle, short yangle, short zangle);

void vec_cross_prod(struct M31 *vecr, struct M31 *vec1, struct M31 *vec2);
void object_vec_normalisation(struct M33 *vec, ubyte col);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
