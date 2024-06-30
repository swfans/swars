/******************************************************************************/
// Bullfrog Engine Smacker Playback Library - for use to remake classic games
// like Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file smack2ail.c
 *     Interface for bounding the Smacer library to AIL interface.
 * @par Purpose:
 *     Allows to easily play audio paths from SMK videos through AIL interface.
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
#include "smack2ail.h"

/******************************************************************************/

/******************************************************************************/

#if 0
uint8_t RADAPI SMACKSOUNDUSEMSS(uint32_t freq, void *digdrv)
{
    if (SmackTimerReadAddr)
        return 0;
    SmackMSSDigDriver = digdrv;
    if (freq < 200)
        freq = 200;
    MSSSpeed = freq;
    LowSoundOpenAddr = MSSLOWSOUNDOPEN;
    LowSoundCloseAddr = MSSLOWSOUNDCLOSE;
    LowSoundPlayedAddr = MSSLOWSOUNDPLAYED;
    LowSoundPurgeAddr = MSSLOWSOUNDPURGE;
    LowSoundOffAddr = MSSLOWSOUNDPURGE;
    SmackTimerSetupAddr = MSSSMACKTIMERSETUP;
    SmackTimerReadAddr = MSSSMACKTIMERREAD;
    SmackTimerDoneAddr = MSSSMACKTIMERDONE;
    MSSTimerPeriod = 1193181 / freq;
    LowSoundCheckAddr = MSSLOWSOUNDCHECK;
    LowSoundOnAddr = NULL;
    LowSoundVolPanAddr = MSSLOWSOUNDVOLPAN;
    return 1;
}
#endif

/******************************************************************************/
