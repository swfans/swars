/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file enginpeff.h
 *     Header file for enginpeff.c.
 * @par Purpose:
 *     Engine scene post-processing effects.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     29 Oct 2023 - 02 May 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef ENGINPEFF_H
#define ENGINPEFF_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

#pragma pack()
/******************************************************************************/
extern ushort gamep_scene_effect;


void scene_post_effect_prepare(void);
void scene_post_effect_for_bucket(short bckt);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif