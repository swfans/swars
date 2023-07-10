/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfini.h
 *     Header file for gini.c.
 * @par Purpose:
 *     Implementation of INI File Format parser.
 * @par Comment:
 *     Allows reading INI initialization configuration files.
 * @author   Tomasz Lis
 * @date     30 Jan 2009 - 05 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_BFINI_H_
#define BFLIBRARY_BFINI_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

/** Named enumeration item, used for INI file parsing. */
struct TbNamedEnum {
    const char *name;
    int num;
};

/** State of an INI buffer parsing. */
struct TbIniParser {
    /** Current position of parsing within the buf. */
    long pos;
    /** Line number, used when loading text files. */
    ulong line_num;
    /** Parsed input string buffer. */
    const char *buf;
    /** Parsed input string buffer length. */
    long buflen;
};

#pragma pack()

/**
 * Prepares state structure for parsing INI buffer.
 *
 * @param parser The parser state structure to initialize.
 * @param buf The input string buffer.
 * @param buflen Length of the input string buffer.
 * @return Gives Lb_SUCCESS on proper initialization.
 */
TbResult LbIniParseStart(struct TbIniParser *parser, const char *buf, long buflen);

/**
 * Marks end of use of state structure for parsing INI buffer.
 *
 * @param parser The parser state structure to finalize.
 * @return Gives Lb_SUCCESS on proper finalization.
 */
TbResult LbIniParseEnd(struct TbIniParser *parser);

/**
 * Parses INI buffer, skipping current position to after next EOLN.
 *
 * @param parser The parser state defining input buffer and current position.
 * @return Gives True if there is unparsed data remaining in the input buffer, false if the end was reached.
 */
TbBool LbIniSkipToNextLine(struct TbIniParser *parser);

/**
 * Searches for start of INI file section with given name.
 *
 * @param parser The parser state defining input buffer and current position.
 * @param sectname String name of the section to search for.
 * @return Returns Lb_SUCCESS if the block is found, Lb_FAIL if buffer exceeded.
 */
TbResult LbIniFindSection(struct TbIniParser *parser, const char *sectname);

/**
 * Parses INI buffer, recognizing a key from given list.
 *
 * @param parser The parser state defining input buffer and current position.
 * @param keylist List of expected INI keys in current section.
 * @return If positive integer is returned, it is the command number recognized in the line.
 * If 0 is returned, that means the current line did not contained any key-value pair and should be skipped.
 * If -1 is returned, that means we have reached end of file.
 * If -2 is returned, that means the key wasn't recognized.
 * If -3 is returned, that means we have reached end of the INI section.
 */
int LbIniRecognizeKey(struct TbIniParser *parser, const struct TbNamedEnum keylist[]);

/**
 * Parses INI value and fills destination buffer with the whole string, that is until EOLN.
 *
 * @param parser The parser state defining input buffer and current position.
 * @param dst The destination string buffer.
 * @param dstlen Length of the destination string buffer.
 * @return Gives length of the string written to dst, or 0 if nothing got written.
 */
int LbIniValueGetStrWhole(struct TbIniParser *parser, char *dst, long dstlen);

/**
 * Parses INI value and fills destination buffer with the next word string.
 *
 * @param parser The parser state defining input buffer and current position.
 * @param dst The destination string buffer.
 * @param dstlen Length of the destination string buffer.
 * @return Gives length of the string written to dst, or 0 if nothing got written.
 */
int LbIniValueGetStrWord(struct TbIniParser *parser, char *dst, long dstlen);

/**
 * Parses INI value and fills destination buffer with the next long integer.
 *
 * The value parsed is contained in one word. Binary and hex strings are supported.
 *
 * @param parser The parser state defining input buffer and current position.
 * @param dst Reference to the destination long value.
 * @return Gives amount of number characters parsed, possibly 0 if there was no number.
 */
int LbIniValueGetLongInt(struct TbIniParser *parser, long *dst);

/**
 * Parses INI value and if it stores enum item name, returns the enumeration item number.
 *
 * @param parser The parser state defining input buffer and current position.
 * @param vallist Array of named enumerations, ended with empty entry.
 * @return Gives enumeration item number, or 0 if not found.
 */
int LbIniValueGetNamedEnum(struct TbIniParser *parser, const struct TbNamedEnum vallist[]);

/**
 * Get name of enum member with given number.
 *
 * @param nelist Array of named enumerations, ended with empty entry.
 * @param num Enumeration item number.
 * @return Gives string with name of enum member, or empty string.
 */
const char *LbNamedEnumGetName(const struct TbNamedEnum nelist[], int num);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFINI_H_
/******************************************************************************/
