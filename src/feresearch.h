/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file feresearch.h
 *     Header file for feresearch.c.
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
#ifndef FERESEARCH_H
#define FERESEARCH_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)


#pragma pack()
/******************************************************************************/

/** Progresses weapon research by one day.
 * @return Returns if something notable happened in regard to the research,
 *   like scientist lost or research completed.
 */
TbBool research_weapon_daily_progress(void);

/** Progresses cybernetic mod research by one day.
 * @return Returns if something notable happened in regard to the research,
 *   like scientist lost or research completed.
 */
TbBool research_cybmod_daily_progress(void);

void forward_research_progress_after_mission(int num_days);

void show_research_screen(void);

void init_research_screen_boxes(void);

/** Reset players part of research screen state.
 *
 * Resets all parts of research screen which are set by the player
 * or related to players game progress.
 */
void reset_research_screen_player_state(void);

void reset_research_screen_boxes_flags(void);
void set_flag01_research_screen_boxes(void);
void clear_research_screen(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
