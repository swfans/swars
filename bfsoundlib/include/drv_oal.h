/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file drv_oal.h
 *     Header file for drv_oal.c.
 * @par Purpose:
 *     OpenAL specific functions.
 * @par Comment:
 *     None.
 * @author   Gynvael Coldwind
 * @author   Unavowed
 * @date     12 Jan 2010 - 05 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef DRV_OAL_H_
#define DRV_OAL_H_

#include "mssal.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
int32_t OPENAL_startup(void);
int32_t OPENAL_shutdown(void);

int oal_sound_initialise(void);
void oal_sound_finalise(void);

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // DRV_OAL_H_
