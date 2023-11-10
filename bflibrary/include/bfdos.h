/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfdos.h
 *     Header with basic DOS type definitions.
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
#ifndef BFLIBRARY_BFDOS_H_
#define BFLIBRARY_BFDOS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#pragma pack(1)

/** DOS Segment Registers.
 */
struct SREGS { // sizeof=12
    u16 es; // offset=0
    u16 cs; // offset=2
    u16 ss; // offset=4
    u16 ds; // offset=6
    u16 fs; // offset=8
    u16 gs; // offset=10
};

typedef struct SREGS SREGS;

/** DOS 32-bit Registers.
 */
struct DWORDREGS { // sizeof=28
    u32 eax; // offset=0
    u32 ebx; // offset=4
    u32 ecx; // offset=8
    u32 edx; // offset=12
    u32 esi; // offset=16
    u32 edi; // offset=20
    u32 cflag; // offset=24
};

typedef struct DWORDREGS DWORDREGS;

/** DOS 16-bit Registers.
 */
struct WORDREGS { // sizeof=28
    u16 ax; // offset=0
    u16 _1; // offset=2
    u16 bx; // offset=4
    u16 _2; // offset=6
    u16 cx; // offset=8
    u16 _3; // offset=10
    u16 dx; // offset=12
    u16 _4; // offset=14
    u16 si; // offset=16
    u16 _5; // offset=18
    u16 di; // offset=20
    u16 _6; // offset=22
    u32 cflag; // offset=24
};

typedef struct WORDREGS WORDREGS;

/** DOS 8-bit Registers.
 */
struct BYTEREGS { // sizeof=16
    u8 al; // offset=0
    u8 ah; // offset=1
    u16 _1; // offset=2
    u8 bl; // offset=4
    u8 bh; // offset=5
    u16 _2; // offset=6
    u8 cl; // offset=8
    u8 ch; // offset=9
    u16 _3; // offset=10
    u8 dl; // offset=12
    u8 dh; // offset=13
    u16 _4; // offset=14
};

typedef struct BYTEREGS BYTEREGS;

/** DOS Registers.
 */
union REGS { // sizeof=28
    BYTEREGS h; // offset=0
    WORDREGS w; // offset=1
    DWORDREGS x; // offset=2
};

/** DOS registers for DPMI interrupts (int 0x31).
 */
struct DPMI_REGS {
    u32 edi;
    u32 esi;
    u32 ebp;
    u32 reserved_by_system;
    u32 ebx;
    u32 edx;
    u32 ecx;
    u32 eax;
    u16 flags;
    u16 es,ds,fs,gs,ip,cs,sp,ss;
};

typedef struct DPMI_REGS DPMI_REGS;

#pragma pack()


#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFDOS_H_
/******************************************************************************/
