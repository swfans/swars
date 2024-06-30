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

#include <stddef.h>
#include "bftime.h"
/******************************************************************************/
extern void *SmackMSSDigDriver;
extern uint32_t MSSSpeed;
extern uint32_t MSSTimerPeriod;

extern uint8_t RADAPI (*LowSoundOpenAddr)(uint8_t, SmackSndTrk *);
extern void RADAPI (*LowSoundCloseAddr)(SmackSndTrk *);
extern uint32_t RADAPI (*LowSoundPlayedAddr)(SmackSndTrk *);
extern void RADAPI (*LowSoundPurgeAddr)(SmackSndTrk *);
extern void RADAPI (*LowSoundOffAddr)(SmackSndTrk *);
extern void RADAPI (*SmackTimerSetupAddr)(void);
extern uint32_t RADAPI (*SmackTimerReadAddr)(void);
extern void RADAPI (*SmackTimerDoneAddr)(void);
extern void RADAPI (*LowSoundCheckAddr)(void);
extern void RADAPI (*LowSoundOnAddr)(void);
extern void RADAPI (*LowSoundVolPanAddr)(uint32_t, uint32_t, SmackSndTrk *);

/******************************************************************************/
uint8_t RADAPI MSSLOWSOUNDOPEN(uint8_t flags, SmackSndTrk *sstrk);
void RADAPI MSSLOWSOUNDCLOSE(SmackSndTrk *sstrk);
uint32_t RADAPI MSSLOWSOUNDPLAYED(SmackSndTrk *sstrk);
void RADAPI MSSLOWSOUNDPURGE(SmackSndTrk *sstrk);
void RADAPI MSSSMACKTIMERSETUP(void);
void RADAPI MSSSMACKTIMERDONE(void);
void RADAPI MSSLOWSOUNDCHECK(void);
void RADAPI MSSLOWSOUNDVOLPAN(uint32_t pan, uint32_t volume, SmackSndTrk *sstrk);

uint32_t RADAPI MSSSMACKTIMERREAD(void)
{
    return LbTimerClock();
}

uint8_t RADAPI SMACKSOUNDUSEMSS(uint32_t speed, void *digdrv)
{
#if 1
    uint8_t ret;
    asm volatile (
      "push %2\n"
      "push %1\n"
      "call ASM_SMACKSOUNDUSEMSS\n"
        : "=r" (ret) : "g" (speed), "g" (digdrv));
    return ret;
#endif
    if (SmackTimerReadAddr)
        return 0;
    SmackMSSDigDriver = digdrv;
    if (speed < 200)
        speed = 200;
    MSSSpeed = speed;
    LowSoundOpenAddr = MSSLOWSOUNDOPEN;
    LowSoundCloseAddr = MSSLOWSOUNDCLOSE;
    LowSoundPlayedAddr = MSSLOWSOUNDPLAYED;
    LowSoundPurgeAddr = MSSLOWSOUNDPURGE;
    LowSoundOffAddr = MSSLOWSOUNDPURGE;
    SmackTimerSetupAddr = MSSSMACKTIMERSETUP;
    SmackTimerReadAddr = MSSSMACKTIMERREAD;
    SmackTimerDoneAddr = MSSSMACKTIMERDONE;
    MSSTimerPeriod = 1193181 / speed;
    LowSoundCheckAddr = MSSLOWSOUNDCHECK;
    LowSoundOnAddr = NULL;
    LowSoundVolPanAddr = MSSLOWSOUNDVOLPAN;
    return 1;
}

/******************************************************************************/
