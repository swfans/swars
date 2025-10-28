/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file felogin.h
 *     Header file for felogin.c.
 * @par Purpose:
 *     Front-end desktop and menu system, login screen.
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
#ifndef FELOGIN_H
#define FELOGIN_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/

ubyte show_login_screen(void);
void init_login_screen_boxes(void);
void reset_login_screen_boxes_flags(void);
void set_flag01_login_screen_boxes(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
