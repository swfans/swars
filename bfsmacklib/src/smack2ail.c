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
#include <time.h>
#include "bftime.h"
#include "bfwindows.h"
#include "aildebug.h"
#include "memfile.h"
/******************************************************************************/
extern void *SmackMSSDigDriver;
extern uint32_t MSSSpeed;
extern uint32_t MSSTimerPeriod;

extern uint32_t timeradjust;
extern uint32_t mss_i_count;
extern uint32_t sndinit;
extern uint32_t msstimer;

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
void RADAPI MSSSMACKTIMERSETUP(void);


uint8_t RADAPI MSSLOWSOUNDOPEN(uint8_t flags, SmackSndTrk *sstrk)
{
    uint8_t ret;
    asm volatile (
      "push %2\n"
      "push %1\n"
      "call ASM_MSSLOWSOUNDOPEN\n"
        : "=r" (ret) : "g" (flags), "g" (sstrk));
    return ret;
}

void RADAPI MSSLOWSOUNDCLOSE(SmackSndTrk *sstrk)
{
    asm volatile (
      "push %0\n"
      "call ASM_MSSLOWSOUNDCLOSE\n"
        :  : "g" (sstrk));
    return;
}

void RADAPI MSSLOWSOUNDCHECK(void)
{
    asm volatile (
      "call ASM_MSSLOWSOUNDCHECK\n"
        :  : );
    return;
}

uint32_t RADAPI MSSLOWSOUNDPLAYED(SmackSndTrk *sstrk)
{
#if 0
    uint32_t ret;
    asm volatile (
      "push %1\n"
      "call ASM_MSSLOWSOUNDPLAYED\n"
        : "=r" (ret) : "g" (sstrk));
    return ret;
#endif
    uint32_t dt;

    MSSLOWSOUNDCHECK();
    dt = (SmackTimerReadAddr() - sstrk->field_6C)
      * (uint64_t)sstrk->field_14 / 1000;
    if (dt > sstrk->field_68)
        dt = sstrk->field_68;
    return sstrk->field_50 + dt;
}

void RADAPI MSSLOWSOUNDPURGE(SmackSndTrk *sstrk)
{
    asm volatile (
      "push %0\n"
      "call ASM_MSSLOWSOUNDPURGE\n"
        :  : "g" (sstrk));
    return;
}

/** Timer routine when linked to AIL; returns a timer value, in miliseconds.
 */
uint32_t RADAPI MSSSMACKTIMERREAD(void)
{
    return LbTimerClock();
}

void RADAPI MSSSMACKTIMERDONE(void)
{
#if 0
    asm volatile (
      "call ASM_MSSSMACKTIMERDONE\n"
        :  : );
    return;
#endif
    if (--sndinit != 0)
        return;
    AIL_release_timer_handle(msstimer);
    AIL_VMM_unlock_range(MSSSMACKTIMERREAD, MSSSMACKTIMERSETUP);
    AIL_vmm_unlock(&mss_i_count, sizeof(mss_i_count));
}

void mss_int(void *clientval)
{
    mss_i_count++;
}

void RADAPI MSSSMACKTIMERSETUP(void)
{
#if 0
    asm volatile (
      "call ASM_MSSSMACKTIMERSETUP\n"
        :  : );
    return;
#endif
    if (++sndinit != 1)
        return;
    AIL_VMM_lock_range(MSSSMACKTIMERREAD, MSSSMACKTIMERSETUP);
    AIL_vmm_lock(&mss_i_count, sizeof(mss_i_count));
    msstimer = AIL_register_timer(mss_int);
    AIL_set_timer_frequency(msstimer, MSSSpeed);
    AIL_start_timer(msstimer);
#if defined(DOS)||defined(GO32)
    uint16_t divsr;
    divsr = AIL_interrupt_divisor();
    // Configure 82C54 timer
    outb(0x43, 0x34);
    outb(0x40, (divsr) & 0xFF);
    outb(0x40, (divsr >> 8) & 0xFF);
#endif
    // Wait for the timers first tick
    uint32_t volatile prev_i_count;
    prev_i_count = mss_i_count;
    while (prev_i_count == mss_i_count) {
        LbWindowsControl();
    }
}

void RADAPI MSSLOWSOUNDVOLPAN(uint32_t pan, uint32_t volume, SmackSndTrk *sstrk)
{
#if 0
    asm volatile (
      "push %2\n"
      "push %1\n"
      "push %0\n"
      "call ASM_MSSLOWSOUNDVOLPAN\n"
        :  : "g" (pan), "g" (volume), "g" (sstrk));
    return;
#endif
    if (volume > 0x8000)
        volume = 0x8000;
    AIL_set_sample_volume(sstrk->smp, ((unsigned int)(1270 * (uint64_t)volume / 0x8000) + 5) / 10);
    AIL_set_sample_pan(sstrk->smp, ((unsigned int)(1270 * (uint64_t)pan / 0x10000) + 5) / 10);
}

/** Default timer routine; returns a timer value, in miliseconds.
 */
uint32_t RADAPI DEFSMACKTIMERREAD(void)
{
#if defined(DOS)||defined(GO32)
    // Every 82C54 timer tick (18.2 per second) activates INT 08h,
    // which increases this value by 1.
    uint32_t tick = PEEKL(0x46C);
    if (tick < lasttimerread)
        timeradjust += lasttimerread - tick;
    lasttimerread = tick;
    return 2746 * (uint64_t)(timeradjust + tick) / 50;
#else
    clock_t tick = clock();
    return 1000 * (uint64_t)tick / CLOCKS_PER_SEC;
#endif
}

/** Default timer finish routine.
 */
void RADAPI DEFSMACKTIMERDONE(void)
{
}

uint32_t RADAPI ac_DEFSMACKTIMERREAD(void);
void RADAPI ac_DEFSMACKTIMERDONE(void);

/** Default timer initialization routine.
 */
void RADAPI DEFSMACKTIMERSETUP(void)
{
    SmackTimerReadAddr = ac_DEFSMACKTIMERREAD;
    SmackTimerDoneAddr = ac_DEFSMACKTIMERDONE;
#if defined(DOS)||defined(GO32)
    uint32_t tick = PEEKL(0x46C);
    timeradjust = -tick;
    // Setup 82C54 timer tick to count to 65536 (results in 18.2 IRQs per second)
    outb(0x43, 0x34);
    outb(0x40, 0);
    outb(0x40, 0);
#endif
}

/* define asm-to-c functions to set as callbacks */
uint8_t RADAPI ac_MSSLOWSOUNDOPEN(uint8_t flags, SmackSndTrk *sstrk);
void RADAPI ac_MSSLOWSOUNDCLOSE(SmackSndTrk *sstrk);
uint32_t RADAPI ac_MSSLOWSOUNDPLAYED(SmackSndTrk *sstrk);
void RADAPI ac_MSSLOWSOUNDPURGE(SmackSndTrk *sstrk);
void RADAPI ac_MSSSMACKTIMERSETUP(void);
void RADAPI ac_MSSSMACKTIMERDONE(void);
void RADAPI ac_MSSLOWSOUNDCHECK(void);
void RADAPI ac_MSSLOWSOUNDVOLPAN(uint32_t pan, uint32_t volume, SmackSndTrk *sstrk);
uint32_t RADAPI ac_MSSSMACKTIMERREAD(void);


uint8_t RADAPI SMACKSOUNDUSEMSS(uint32_t speed, void *digdrv)
{
#if 0
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
    LowSoundOpenAddr = ac_MSSLOWSOUNDOPEN;
    LowSoundCloseAddr = ac_MSSLOWSOUNDCLOSE;
    LowSoundPlayedAddr = ac_MSSLOWSOUNDPLAYED;
    LowSoundPurgeAddr = ac_MSSLOWSOUNDPURGE;
    LowSoundOffAddr = ac_MSSLOWSOUNDPURGE;
    SmackTimerSetupAddr = ac_MSSSMACKTIMERSETUP;
    SmackTimerReadAddr = ac_MSSSMACKTIMERREAD;
    SmackTimerDoneAddr = ac_MSSSMACKTIMERDONE;
    MSSTimerPeriod = 1193181 / speed;
    LowSoundCheckAddr = ac_MSSLOWSOUNDCHECK;
    LowSoundOnAddr = NULL;
    LowSoundVolPanAddr = ac_MSSLOWSOUNDVOLPAN;
    return 1;
}

/******************************************************************************/
