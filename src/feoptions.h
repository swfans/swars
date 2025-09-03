/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file feoptions.h
 *     Header file for feoptions.c.
 * @par Purpose:
 *     Front-end desktop and menu system, options screen.
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
#ifndef FEOPTIONS_H
#define FEOPTIONS_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/
extern char options_title_text[];

void init_options_audio_screen_boxes(void);
void init_options_visual_screen_boxes(void);
void update_options_screen_state(void);
void reset_options_audio_boxes_flags(void);
void reset_options_visual_boxes_flags(void);
void set_flag02_audio_screen_boxes(void);
void set_flag02_gfx_screen_boxes(void);

ubyte show_options_audio_screen(void);
ubyte show_options_visual_screen(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
