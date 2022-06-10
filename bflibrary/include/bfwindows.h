/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfwindows.h
 *     Header file for swindows.cpp.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_SWINDOWS_H_
#define BFLIBRARY_SWINDOWS_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LB_LARGE_DELAY_TIME 20

/**
 * Informs if the library general preinitialization was called.
 */
extern TbBool lbLibInitialised;

/** Initialise the work with bullfrog library before use.
 *
 * The routine resets any config variables related to screen, and
 * prepares everything for use of LbScreenSetupAnyMode().
 * It performs any screen-related initialization which don't have to
 * be repeated on every mode change.
 */
TbResult LbBaseInitialise(void);

TbResult LbBaseReset(void);

void LbDoMultitasking(void);

int LbWindowsControl();

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_SWINDOWS_H_
/******************************************************************************/
