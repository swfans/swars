/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file linksmk.h
 *     Header file for linksmk.c.
 * @par Purpose:
 *     Link between the main game and smacker playback library.
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
#ifndef LINKSMK_H
#define LINKSMK_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void smack_malloc_setup(void);
void smack_malloc_free_all(void);

#ifdef __cplusplus
};
#endif

#endif // LINKSMK_H
/******************************************************************************/
