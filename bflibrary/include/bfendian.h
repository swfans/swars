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

/** Return the big-endian longword at p.
 */
unsigned long blong(unsigned char *p);

/** Return the little-endian longword at p.
 */
unsigned long llong(unsigned char *p);

/** Return the big-endian word at p.
 */
unsigned long bword(unsigned char *p);

/* Return the little-endian word at p.
 */
unsigned long lword(unsigned char *p);

/**
 * Toggles a masked bit in the flags field to the value.
 * This version assumes the flag field is 1 byte long.
 * @param flags Pointer to the flags byte.
 * @param mask Bitmask for the flag.
 */
void toggle_flag_byte(unsigned char *flags, unsigned char mask);

/**
 * Toggles a masked bit in the flags field to the value.
 * This version assumes the flag field is 4 bytes long.
 * @param flags Pointer to the flags byte.
 * @param mask Bitmask for the flag.
 */
void toggle_flag_dword(unsigned long *flags, unsigned long mask);

/**
 * Sets a masked bit in the flags field to the value.
 * This version assumes the flag field is 2 bytes long.
 * @param flags Pointer to the flags byte.
 * @param mask Bitmask for the flag.
 * @param value The new logic value.
 */
void set_flag_word(unsigned short *flags, unsigned short mask, short value);

/**
 * Sets a masked bit in the flags field to the value.
 * This version assumes the flag field is 1 byte long.
 * @param flags Pointer to the flags byte.
 * @param mask Bitmask for the flag.
 * @param value The new logic value.
 */
void set_flag_byte(unsigned char *flags, unsigned char mask, short value);

/**
 * Sets a masked bit in the flags field to the value.
 * This version assumes the flag field is 4 bytes long.
 * @param flags Pointer to the flags byte.
 * @param mask Bitmask for the flag.
 * @param value The new logic value.
 */
void set_flag_dword(unsigned long *flags, unsigned long mask, short value);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFENDIAN_H_
/******************************************************************************/
