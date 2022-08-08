/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file helpers_frame.h
 *     Header file for all test frames.
 * @par Purpose:
 *     Single frames from games, as helper functions for tests.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     05 Nov 2021 - 29 Jul 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef HELPERS_FRAME_H_
#define HELPERS_FRAME_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*TestFrameFunc)(void);

void test_frame_swars01(void);

#ifdef __cplusplus
};
#endif

#endif // HELPERS_FRAME_H_
/******************************************************************************/
