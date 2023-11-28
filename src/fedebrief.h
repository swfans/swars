/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file fedebrief.h
 *     Header file for fedebrief.c.
 * @par Purpose:
 *     Front-end desktop and menu system, mission debrief screen.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef FEDEBRIEF_H
#define FEDEBRIEF_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/

void init_debrief_screen_boxes(void);
void set_flag01_debrief_screen_boxes(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
