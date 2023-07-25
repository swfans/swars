/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file campaign.h
 *     Header file for campaign.c.
 * @par Purpose:
 *     Campaign progress and missions definitions.
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
#ifndef CAMPAIGN_H
#define CAMPAIGN_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/
extern struct Objective *game_used_objectives;
extern ushort next_used_objective; // = 1;
extern struct Objective *game_objectives;
extern ushort next_objective;

void load_missions(int num);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
