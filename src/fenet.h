/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file fenet.h
 *     Header file for fenet.c.
 * @par Purpose:
 *     Front-end desktop and menu system, multiplayer screen.
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
#ifndef FENET_H
#define FENET_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/

void init_net_screen_boxes(void);
void show_netgame_unkn_case1(void);
void reset_net_screen_boxes_flags(void);
void reset_net_screen_EJECT_flags(void);
void set_flag01_net_screen_boxes(void);
void set_flag02_net_screen_boxes(void);
void switch_net_screen_boxes_to_initiate(void);
void switch_net_screen_boxes_to_execute(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
