/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file fecntrls.h
 *     Header file for fecntrls.c.
 * @par Purpose:
 *     Front-end desktop and menu system, research screen.
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
#ifndef FECNTRLS_H
#define FECNTRLS_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

#pragma pack()
/******************************************************************************/
/** Game key currently being edited in the controls screen.
 * Max value is 2x max GameKey index - because it also stored distinction
 * between entering keyboard key and joystick key.
 */
extern ubyte controls_edited_gkey;

void init_controls_screen_boxes(void);
void reset_controls_screen_boxes_flags(void);
ubyte show_options_controls_screen(void);
void set_flag01_controls_screen_boxes(void);
void set_flag02_controls_screen_boxes(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
