/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfendian.h
 *     Header file for gendian.c.
 * @par Purpose:
 *     Integer endianness convert function, and bit setting.
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
#ifndef BFLIBRARY_BFENDIAN_H_
#define BFLIBRARY_BFENDIAN_H_

#ifdef __cplusplus
extern "C" {
#endif

unsigned long blong(unsigned char *p);
unsigned long llong(unsigned char *p);
unsigned long bword(unsigned char *p);
unsigned long lword(unsigned char *p);
void set_flag_byte(unsigned char *flags, unsigned char mask, short value);
void set_flag_word(unsigned short *flags, unsigned short mask, short value);
void set_flag_dword(unsigned long *flags, unsigned long mask, short value);
void toggle_flag_byte(unsigned char *flags, unsigned char mask);
void toggle_flag_dword(unsigned long *flags, unsigned long mask);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFENDIAN_H_
/******************************************************************************/
