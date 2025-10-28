/******************************************************************************/
// Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file wadfile.c
 *     Support for WAD aggregate files.
 * @par Purpose:
 *     Support for WAD files, used as an aggregate for a set of small files.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 19 Aug 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "wadfile.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "bftypes.h"

#include "campaign.h"
#include "game_data.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/

TbResult wadfile_format_entfile_name(char *entfile, const char *filename)
{
    const char *only_fname;
    int i;

    only_fname = strrchr(filename, '/');
    if (only_fname != NULL)
        only_fname++;
    else
        only_fname = filename;

    for (i = 0; only_fname[i] != '\0'; i++)
    {
        // Does not make sense to format WAD entry name longer than size within index
        if (i >= (int)sizeof(((struct WADIndexEntry *)0)->Filename)) {
            entfile[i-1] = '\0';
            return Lb_FAIL;
        }
        entfile[i] = toupper(only_fname[i]);
    }
    entfile[i] = '\0';

    return Lb_SUCCESS;
}

TbResult wadfile_find_index_entry(struct WADIndexEntry *fentry, const char *wadfile, const char *entfile)
{
    char locfname[DISKPATH_SIZE];
    TbFileHandle fh;
    int nread;

    sprintf(locfname, "%s.idx", wadfile);
    fh = LbFileOpen(locfname, Lb_FILE_MODE_READ_ONLY);
    if (fh == INVALID_FILE)
        return Lb_FAIL;

    do {
        nread = LbFileRead(fh, fentry, sizeof(struct WADIndexEntry));
    } while ((strcmp(entfile, fentry->Filename) != 0) &&
      (nread == sizeof(struct WADIndexEntry)));

    LbFileClose(fh);

    if (nread != sizeof(struct WADIndexEntry))
        return Lb_FAIL;

    return Lb_SUCCESS;
}

TbFileHandle open_file_from_wad(const char *filename, const char *wadfile)
{
#if 0
    TbFileHandle ret;
    asm volatile ("call ASM_open_file_from_wad\n"
        : "=r" (ret) : "a" (filename), "d" (wadfile));
    return ret;
#else
    char locfname[DISKPATH_SIZE];
    char locstr[64];
    struct WADIndexEntry fentry;
    TbFileHandle fh;
    TbResult ret;

    ret = wadfile_format_entfile_name(locstr, filename);
    if (ret != Lb_SUCCESS)
        return INVALID_FILE;

    ret = wadfile_find_index_entry(&fentry, wadfile, locstr);
    if (ret != Lb_SUCCESS)
        return INVALID_FILE;

    sprintf(locfname, "%s.wad", wadfile);
    fh = LbFileOpen(locfname, Lb_FILE_MODE_READ_ONLY);
    if (fh == INVALID_FILE)
        return fh;

    LbFileSeek(fh, fentry.Offset, Lb_FILE_SEEK_BEGINNING);
    return fh;
#endif
}

int load_file_wad(const char *filename, const char *wadfile, void *outbuf)
{
    char locfname[DISKPATH_SIZE];
    char locstr[64];
    struct WADIndexEntry fentry;
    TbFileHandle fh;
    TbResult ret;
    long nread;

    ret = wadfile_format_entfile_name(locstr, filename);
    if (ret != Lb_SUCCESS)
        return -1;

    ret = wadfile_find_index_entry(&fentry, wadfile, locstr);
    if (ret != Lb_SUCCESS)
        return -1;

    sprintf(locfname, "%s.wad", wadfile);
    fh = LbFileOpen(locfname, Lb_FILE_MODE_READ_ONLY);
    if (fh == INVALID_FILE)
        return -1;

    LbFileSeek(fh, fentry.Offset, Lb_FILE_SEEK_BEGINNING);
    nread = LbFileRead(fh, outbuf, fentry.Length);
    LbFileClose(fh);
    return nread;
}

int load_file_alltext(const char *filename, void *outbuf)
{
    char wadfile[DISKPATH_SIZE];
    struct Campaign *p_campgn;
    const char *campgn_mark;
    PathInfo *pinfo;

    pinfo = &game_dirs[DirPlace_LangData];
    p_campgn = &campaigns[background_type];
    campgn_mark = p_campgn->TextFnMk;
    LOGNO("Load \"%s\" from wad dir=\"%s\" mark=\"%s\"", filename, pinfo->directory, campgn_mark);

    snprintf(wadfile, DISKPATH_SIZE, "%s/alltext-%s", pinfo->directory, campgn_mark);
    return load_file_wad(filename, wadfile, outbuf);
}

/******************************************************************************/
