/******************************************************************************/
// Bullfrog Engine Smacker Playback Library - for use to remake classic games
// like Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file smack2ail.h
 *     Header file for smack2ail.c.
 * @par Purpose:
 *     External interface of the smacker playback library.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2011 - 13 Jun 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFSMACKLIB_SMACK2AIL_H_
#define BFSMACKLIB_SMACK2AIL_H_

#include <stdint.h>
#include "smack.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/


/******************************************************************************/

/** Link Smacker audio output to AIL library interface.
 *
 * @param speed Frequency of the timer sending buffers to AIL output.
 * @param digdrv Digital Audio Driver instance from the AIL library.
 */
uint8_t RADAPI SMACKSOUNDUSEMSS(uint32_t speed, void *digdrv);

/******************************************************************************/

#ifdef __cplusplus
};
#endif

#endif // BFSMACKLIB_SMACK2AIL_H_
/******************************************************************************/
