/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file wrcities.c
 *     Routines for handling a list of cities for world map.
 * @par Purpose:
 *     Implement functions for parsing cities file and handling cities array.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 11 Sep 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "wrcities.h"

#include <string.h>
#include <stdlib.h>
#include "bffile.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bfini.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/

enum CitiesConfigParam {
    CtC_CityCount = 1,
    CtC_Name,
    CtC_Coord,
    CtC_MapNo,
    CtC_Flags,
};

const struct TbNamedEnum cities_conf_common_cmds[] = {
  {"CityCount",	CtC_CityCount},
  {NULL,		0},
};

const struct TbNamedEnum cities_conf_city_cmds[] = {
  {"Name",		CtC_Name},
  {"Coord",		CtC_Coord},
  {"MapNo",		CtC_MapNo},
  {"Flags",		CtC_Flags},
  {NULL,		0},
};

void load_city_txt(void)
{
#if 0
    asm volatile ("call ASM_load_city_txt\n"
        :  :  : "eax" );
#endif
    char *s;
    int i, n, k, city;

    k = load_file_wad("textdata/city.txt", "qdata/alltext", memload);
    if (k == -1)
        return;
    s = (char *)memload;
    // Skip property names for campaigns before current
    for (n = 0; n < background_type; n++)
    {
        for (i = 0; i < 6; )
        {
            if ((*s == '#') || (*s == '\r'))
            {
                char c;
                do {
                    c = *s;
                    s++;
                } while ((c != '\n') && (c != '\0'));
                continue;
            }
            while ((*s != '\r') && (*s != '\0')) {
                s++;
            }
            *s = '\0';
            s += 2;
            i++;
        }
    }
    // Read property names
    {
        for (i = 0; i < 6; )
        {
            if ((*s == '#') || (*s == '\r'))
            {
                char c;
                do {
                    c = *s;
                    s++;
                } while ((c != '\n') && (c != '\0'));
                continue;
            }
            if (i == 0)
                k = 404;
            else
                k = 489+i;
            gui_strings[k] = s;
            while ((*s != '\r') && (*s != '\0')) {
                s++;
            }
            *s = '\0';
            s += 2;
            // String ready, preprocess it
            strupr(gui_strings[k]);
            i++;
        }
    }
    // Skip property names for campaigns after current
    for (n = background_type+1; n < 3; n++)
    {
        for (i = 0; i < 6; )
        {
            if ((*s == '#') || (*s == '\r'))
            {
                char c;
                do {
                    c = *s;
                    s++;
                } while ((c != '\n') && (c != '\0'));
                continue;
            }
            while ((*s != '\r') && (*s != '\0')) {
                s++;
            }
            *s = '\0';
            s += 2;
            i++;
        }
    }
    // Read per-city strings
    for (city = 0; city < num_cities; city++)
    {
        for (i = 0; i < 6; )
        {
            if ((*s == '#') || (*s == '\r'))
            {
                char c;
                do {
                    c = *s;
                    s++;
                } while ((c != '\n') && (c != '\0'));
                continue;
            }
            cities[city].TextIndex[i] = s - (char *)memload;
            while ((*s != '\r') && (*s != '\0')) {
                s++;
            }
            *s = '\0';
            s += 2;
            // String ready, preprocess it
            k = cities[city].TextIndex[i];
            my_preprocess_text((char *)&memload[k]);
            i++;
        }
    }
}

void save_city_single_conf(TbFileHandle fh, struct City *p_city, char *buf)
{
    {
        sprintf(buf, "Name = %s\n", memload + p_city->TextIndex[0]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_city->TextIndex[0] != 0) {
        sprintf(buf, "TextId = %hu\n", p_city->TextIndex[0]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    {
        sprintf(buf, "PropTextId = %hu %hu %hu %hu %hu\n",
          (ushort)p_city->TextIndex[1], (ushort)p_city->TextIndex[2],
          (ushort)p_city->TextIndex[3], (ushort)p_city->TextIndex[4], (ushort)p_city->TextIndex[5]);
        LbFileWrite(fh, buf, strlen(buf));
    }
    {
        sprintf(buf, "Coord = %hd %hd\n", p_city->X, p_city->Y);
        LbFileWrite(fh, buf, strlen(buf));
    }
    {
        sprintf(buf, "MapNo = %hu\n", (ushort)p_city->MapID);
        LbFileWrite(fh, buf, strlen(buf));
    }
    {
        sprintf(buf, "LevelNo = %hu\n", (ushort)p_city->Level);
        LbFileWrite(fh, buf, strlen(buf));
    }
    {
        sprintf(buf, "Flags = %hu\n", (ushort)p_city->Flags);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (p_city->Info) {
        sprintf(buf, "Info = %hu\n", (ushort)p_city->Info);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if ((p_city->Dummy2[0]|p_city->Dummy2[1]|p_city->Dummy2[2]) != 0) {
        sprintf(buf, "Dummy2 = %d %d %d %d %d\n",
          (int)p_city->Dummy2[0], (int)p_city->Dummy2[1], (int)p_city->Dummy2[2],
          (int)p_city->Dummy2[3], (int)p_city->Dummy2[4]);
        LbFileWrite(fh, buf, strlen(buf));
    }
}

void save_cities_conf_file(void)
{
    TbFileHandle fh;
    char locbuf[320];
    char conf_fname[80];
    int i;

    sprintf(conf_fname, "%s" FS_SEP_STR "cities.ini", "conf");
    fh = LbFileOpen(conf_fname, Lb_FILE_MODE_NEW);
    if (fh == INVALID_FILE) {
        LOGERR("Could not create '%s' file.", conf_fname);
        return;
    }

    {
        sprintf(locbuf, "[common]\n");
        LbFileWrite(fh, locbuf, strlen(locbuf));

        sprintf(locbuf, "CityCount = %hu\n", num_cities);
        LbFileWrite(fh, locbuf, strlen(locbuf));

        sprintf(locbuf, "\n");
        LbFileWrite(fh, locbuf, strlen(locbuf));
    }

    for (i = 0; i < num_cities; i++) {
        struct City *p_city;

        p_city = &cities[i];

        sprintf(locbuf, "[city%d]\n", i);
        LbFileWrite(fh, locbuf, strlen(locbuf));

        save_city_single_conf(fh, p_city, locbuf);

        sprintf(locbuf, "\n");
        LbFileWrite(fh, locbuf, strlen(locbuf));
    }
    LbFileClose(fh);
}

void load_city_data(ubyte type)
{
#if 0
    TbFileHandle fh;

    fh = LbFileOpen("data/cities.dat", Lb_FILE_MODE_READ_ONLY);
    if (fh == INVALID_FILE) {
        return;
    }
    LbFileRead(fh, &num_cities, 1);
    LbFileRead(fh, cities, sizeof(struct City) * num_cities);
    LbFileClose(fh);
#else
    read_cities_conf_file();
#endif
}

/** Reads cities file, with information on all cities included.
 */
void read_cities_conf_file(void)
{
    TbFileHandle conf_fh;
    TbBool done;
    int i;
    long k;
    char *conf_buf;
    struct TbIniParser parser;
    char conf_fname[80];
    int conf_len;
    int city_id;

    sprintf(conf_fname, "%s" FS_SEP_STR "cities.ini", "conf");
    conf_fh = LbFileOpen(conf_fname, Lb_FILE_MODE_READ_ONLY);
    if (conf_fh != INVALID_FILE) {
        conf_len = LbFileLengthHandle(conf_fh);
        if (conf_len > 1024*1024)
            conf_len = 1024*1024;
        conf_buf = LbMemoryAlloc(conf_len+16);
        conf_len = LbFileRead(conf_fh, conf_buf, conf_len);
        LOGSYNC("Processing '%s' file, %d bytes", conf_fname, conf_len);
        LbFileClose(conf_fh);
    } else {
        LOGERR("Could not open '%s' file, cities list empty.", conf_fname);
        conf_buf = LbMemoryAlloc(16);
        conf_len = 0;
    }
    conf_buf[conf_len] = '\0';
    LbIniParseStart(&parser, conf_buf, conf_len);
#define CONFWRNLOG(format,args...) LOGWARN("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
#define CONFDBGLOG(format,args...) LOGDBG("%s(line %lu): " format, conf_fname, parser.line_num, ## args)
    num_cities = 0;

    // Parse the [common] section of loaded file
    done = false;
    if (LbIniFindSection(&parser, "common") != Lb_SUCCESS) {
        CONFWRNLOG("Could not find \"[%s]\" section, file skipped.", "common");
        LbIniParseEnd(&parser);
        LbMemoryFree(conf_buf);
        return;
    }
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(cities_conf_common_cmds,cmd_num)
    while (!done)
    {
        int cmd_num;

        // Finding command number in this line
        i = 0;
        cmd_num = LbIniRecognizeKey(&parser, cities_conf_common_cmds);
        // Now store the config item in correct place
        switch (cmd_num)
        {
        case CtC_CityCount:
            i = LbIniValueGetLongInt(&parser, &k);
            if (i <= 0) {
                CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                break;
            }
            num_cities = k;
            CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)num_cities);
            break;
        case 0: // comment
            break;
        case -1: // end of buffer
        case -3: // end of section
            done = true;
            break;
        default:
            CONFWRNLOG("Unrecognized command.");
            break;
        }
        LbIniSkipToNextLine(&parser);
    }
#undef COMMAND_TEXT
    for (city_id = 0; city_id < num_cities; city_id++)
    {
        char sect_name[16];
        struct City *p_city;

        p_city = &cities[city_id];
        LbMemorySet(p_city, 0, sizeof(struct City));

        // Parse the [cityN] sections of loaded file
        done = false;
        sprintf(sect_name, "city%d", city_id);
        if (LbIniFindSection(&parser, sect_name) != Lb_SUCCESS) {
            CONFWRNLOG("Could not find \"[%s]\" section.", sect_name);
            continue;
        }
#define COMMAND_TEXT(cmd_num) LbNamedEnumGetName(cities_conf_city_cmds,cmd_num)
        while (!done)
        {
            int cmd_num;

            // Finding command number in this line
            i = 0;
            cmd_num = LbIniRecognizeKey(&parser, cities_conf_city_cmds);
            // Now store the config item in correct place
            switch (cmd_num)
            {
            case CtC_Name:
#if 0
                i = LbIniValueGetStrWhole(&parser, p_str, 80);
                if (i <= 0) {
                    CONFWRNLOG("Couldn't read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_city->TextName = p_str;
                p_str += strlen(p_str) + 1;
                CONFDBGLOG("%s \"%s\"", COMMAND_TEXT(cmd_num), (int)p_city->TextName);
#endif
                break;
            case CtC_Coord:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter X.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_city->X = k;
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter Y.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_city->Y = k;
                CONFDBGLOG("%s %d %d", COMMAND_TEXT(cmd_num), (int)p_city->X, (int)p_city->Y);
                break;
            case CtC_MapNo:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_city->MapID = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_city->MapID);
                break;
            case CtC_Flags:
                i = LbIniValueGetLongInt(&parser, &k);
                if (i <= 0) {
                    CONFWRNLOG("Could not read \"%s\" command parameter.", COMMAND_TEXT(cmd_num));
                    break;
                }
                p_city->Flags = k;
                CONFDBGLOG("%s %d", COMMAND_TEXT(cmd_num), (int)p_city->Flags);
                break;
            case 0: // comment
                break;
            case -1: // end of buffer
            case -3: // end of section
                done = true;
                break;
            default:
                CONFWRNLOG("Unrecognized command.");
                break;
            }
            LbIniSkipToNextLine(&parser);
        }
#undef COMMAND_TEXT
    }
#undef CONFDBGLOG
#undef CONFWRNLOG
    LbIniParseEnd(&parser);
    LbMemoryFree(conf_buf);
}

/******************************************************************************/
