/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gini.c
 *     Implementation of INI File Format parser.
 * @par Purpose:
 *     Read INI initialization configuration files.
 * @par Comment:
 *     The original library had no utility functions to parse INI files, and
 *     each game implemented a simple one function parser when neccessary.
 *     But the simple implementation lead to issues as it could not recognize
 *     many of standard features of INI format (causing bad parse if a user
 *     modified the file by hand), and was a bit different for each game,
 *     causing confusion. This set of simple functions solves these problems.
 * @author   Tomasz Lis
 * @date     30 Jan 2009 - 05 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "bfini.h"

#include "bftypes.h"
#include "bfstrut.h"
#include "privbflog.h"

TbResult LbIniParseStart(struct TbIniParser *parser, const char *buf, long buflen)
{
    parser->pos = 0;
    parser->line_num = 1;
    parser->buf = buf;
    parser->buflen = buflen;
    return Lb_SUCCESS;
}

TbResult LbIniParseEnd(struct TbIniParser *parser)
{
    parser->pos = parser->buflen;
    return Lb_SUCCESS;
}

TbBool LbIniSkipToNextLine(struct TbIniParser *parser)
{
    // Skip to end of the line
    while (parser->pos < parser->buflen)
    {
        if ((parser->buf[parser->pos] == '\r') ||
            (parser->buf[parser->pos] == '\n'))
            break;
        parser->pos++;
    }
    // Go to start of next line
    while (parser->pos < parser->buflen)
    {
        if ((uchar)parser->buf[parser->pos] > 32)
            break;
        if (parser->buf[parser->pos] == '\n')
            parser->line_num++;
        parser->pos++;
    }
    return (parser->pos < parser->buflen);
}

static TbBool IniSkipSpaces(struct TbIniParser *parser)
{
    while (parser->pos < parser->buflen)
    {
        if ((parser->buf[parser->pos] != ' ') &&
            (parser->buf[parser->pos] != '\t') &&
            (parser->buf[parser->pos] != 26) &&
            ((uchar)parser->buf[parser->pos] >= 7))
            break;
        parser->pos++;
    }
    return (parser->pos < parser->buflen);
}

TbResult LbIniFindSection(struct TbIniParser *parser, const char *sectname)
{
    int blname_len;
    parser->pos = 0;
    parser->line_num = 1;
    blname_len = strlen(sectname);
    while (parser->pos + blname_len + 2 < parser->buflen)
    {
        // Skipping starting spaces
        if (!IniSkipSpaces(parser))
            break;
        // Checking if this line is start of a block
        if (parser->buf[parser->pos] != '[')
        {
            LbIniSkipToNextLine(parser);
            continue;
        }
        parser->pos++;
        // Skipping any spaces
        if (!IniSkipSpaces(parser))
            break;
        if (parser->pos + blname_len + 2 >= parser->buflen)
            break;
        if (strncasecmp(&parser->buf[parser->pos], sectname, blname_len) != 0)
        {
            LbIniSkipToNextLine(parser);
            continue;
        }
        parser->pos += blname_len;
        // Skipping any spaces
        if (!IniSkipSpaces(parser))
            break;
        if (parser->buf[parser->pos] != ']')
        {
            LbIniSkipToNextLine(parser);
            continue;
        }
        LbIniSkipToNextLine(parser);
        return Lb_SUCCESS;
    }
    return Lb_FAIL;
}

int LbIniRecognizeKey(struct TbIniParser *parser, const struct TbNamedEnum keylist[])
{
    int i,cmdname_len;
    LOGNO("Starting");
    if ((parser->pos) >= parser->buflen)
        return -1;
    // Skipping starting spaces
    while ((parser->buf[parser->pos] == ' ') ||
           (parser->buf[parser->pos] == '\t') ||
           (parser->buf[parser->pos] == '\n') ||
           (parser->buf[parser->pos] == '\r') ||
           (parser->buf[parser->pos] == 26) ||
           ((uchar)parser->buf[parser->pos] < 7))
    {
        parser->pos++;
        if (parser->pos >= parser->buflen) return -1;
    }
    // Checking if this line is a comment
    if (parser->buf[parser->pos] == ';')
        return 0;
    // Checking if this line is start of a block
    if (parser->buf[parser->pos] == '[')
        return -3;
    // Finding command number
    i = 0;
    while (keylist[i].num > 0)
    {
        cmdname_len = strlen(keylist[i].name);
        if (parser->pos + cmdname_len > parser->buflen) {
            i++;
            continue;
        }
        // Find a matching command
        if (strncasecmp(&parser->buf[parser->pos], keylist[i].name, cmdname_len) == 0)
        {
            parser->pos += cmdname_len;
            // if we're not at end of input buffer..
            if (parser->pos < parser->buflen)
            {
                // make sure it's whole command, not just start of different one
               if ((parser->buf[parser->pos] != ' ') &&
                   (parser->buf[parser->pos] != '\t') &&
                   (parser->buf[parser->pos] != '=')  &&
                   ((uchar)parser->buf[parser->pos] >= 7))
               {
                  parser->pos -= cmdname_len;
                  i++;
                  continue;
               }
               // Skipping spaces between command and parameters
               while ((parser->buf[parser->pos] == ' ') ||
                  (parser->buf[parser->pos] == '\t') ||
                  (parser->buf[parser->pos] == '=')  ||
                  ((uchar)parser->buf[parser->pos] < 7))
               {
                 parser->pos++;
                 if (parser->pos >= parser->buflen) break;
               }
            }
            return keylist[i].num;
        }
        i++;
    }
    return -2;
}

int LbIniValueGetStrWhole(struct TbIniParser *parser, char *dst, long dstlen)
{
    int i;
    TbBool in_quotes;
    in_quotes = false;
    if (parser->pos >= parser->buflen) return 0;
    // Skipping spaces after previous parameter
    while ((parser->buf[parser->pos] == ' ') || (parser->buf[parser->pos] == '\t'))
    {
        parser->pos++;
        if (parser->pos >= parser->buflen) return 0;
    }
    if (parser->buf[parser->pos] == '\"') {
        in_quotes = true;
        parser->pos++;
        if (parser->pos >= parser->buflen) return 0;
    }
    for (i = 0; i + 1 < dstlen; i++)
    {
        if (in_quotes) {
            if ((parser->buf[parser->pos] == '\"') ||
                 (parser->buf[parser->pos] == '\r') ||
                 (parser->buf[parser->pos] == '\n'))
                break;
        } else {
            if ((parser->buf[parser->pos] == '\r') ||
                (parser->buf[parser->pos] == '\n') ||
                ((uchar)parser->buf[parser->pos] < 7))
              break;
        }
        dst[i] = parser->buf[parser->pos];
        parser->pos++;
        if (parser->pos >= parser->buflen) break;
    }
    dst[i] = '\0';
    return i;
}

int LbIniValueGetStrWord(struct TbIniParser *parser, char *dst, long dstlen)
{
    int i;
    TbBool in_quotes;
    in_quotes = false;
    if (parser->pos >= parser->buflen) return 0;
    // Skipping spaces after previous parameter
    while ((parser->buf[parser->pos] == ' ') || (parser->buf[parser->pos] == '\t'))
    {
        parser->pos++;
        if (parser->pos >= parser->buflen) return 0;
    }
    if (parser->buf[parser->pos] == '\"') {
        in_quotes = true;
        parser->pos++;
        if (parser->pos >= parser->buflen) return 0;
    }
    for (i=0; i+1 < dstlen; i++)
    {
        if (in_quotes) {
            if ((parser->buf[parser->pos] == '\"') ||
                 (parser->buf[parser->pos] == '\r') ||
                 (parser->buf[parser->pos] == '\n'))
                break;
        } else {
            if ((parser->buf[parser->pos] == ' ') ||
                 (parser->buf[parser->pos] == '\t') ||
                 (parser->buf[parser->pos] == '\r') ||
                 (parser->buf[parser->pos] == '\n') ||
                 ((uchar)parser->buf[parser->pos] < 7))
                break;
        }
        dst[i] = parser->buf[parser->pos];
        parser->pos++;
        if (parser->pos >= parser->buflen) {
            i++;
            break;
        }
    }
    dst[i] = '\0';
    return i;
}

int LbIniValueGetLongInt(struct TbIniParser *parser, long *dst)
{
    char buf[32];
    char *bend;
    int i;
    i = LbIniValueGetStrWord(parser, buf, sizeof(buf));
    if (i < 1)
        return i;
    *dst = strtol(buf,&bend,0);
    return (int)(bend - buf);
}

int LbIniValueGetNamedEnum(struct TbIniParser *parser, const struct TbNamedEnum vallist[])
{
    int i;
    int par_len;
    TbBool in_quotes;
    in_quotes = false;
    if (parser->pos >= parser->buflen) return 0;
    // Skipping spaces after previous parameter
    while ((parser->buf[parser->pos] == ' ') ||
           (parser->buf[parser->pos] == '\t'))
    {
        parser->pos++;
        if (parser->pos >= parser->buflen) return 0;
    }
    if (parser->buf[parser->pos] == '\"') {
        in_quotes = true;
        parser->pos++;
        if (parser->pos >= parser->buflen) return 0;
    }
    i = 0;
    while (vallist[i].name != NULL)
    {
        par_len = strlen(vallist[i].name);
        if (strncasecmp(&parser->buf[parser->pos], vallist[i].name, par_len) == 0)
        {
            // If EOLN found, finish and return position before the EOLN
            if ((parser->buf[parser->pos + par_len] == '\n') ||
                (parser->buf[parser->pos + par_len] == '\r'))
            {
              parser->pos += par_len;
              return vallist[i].num;
            }
            if (in_quotes) {
                // If quote ends, finish and return position after the quote
                if (parser->buf[parser->pos + par_len] == '\"')
                {
                    parser->pos += par_len + 1;
                    return vallist[i].num;
                }
            } else {
                // If non-EOLN blank char, finish and return position after the char
                if ((parser->buf[parser->pos + par_len] == ' ') ||
                    (parser->buf[parser->pos + par_len] == '\t') ||
                    ((uchar)parser->buf[parser->pos + par_len] < 7))
                {
                    parser->pos += par_len + 1;
                    return vallist[i].num;
                }
            }
        }
        i++;
    }
    return 0;
}

const char *LbNamedEnumGetName(const struct TbNamedEnum nelist[], int num)
{
    int i;
    i = 0;
    while (nelist[i].name != NULL)
    {
        LOGNO("\"%s\", %d %d", nelist[i].name, nelist[i].num, num);
        if (nelist[i].num == num)
            return nelist[i].name;
        i++;
    }
    return lbEmptyString;
}

/******************************************************************************/
