/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file gdata.c
 *     Reading/freeing of file lists.
 * @par Purpose:
 *     Allows to allocate memory and read multiple files at once.
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
#include <strings.h>
#include "bfdata.h"

#include "bfmemory.h"
#include "bffile.h"
#include "privbflog.h"


ModifyDataLoadFnameFunc modify_data_load_filename_function = &defaultModifyDataLoadFilename;


short LbDataFree(struct TbLoadFiles *load_file)
{
    if (load_file == NULL)
        return 0;
    void **data;
    data = load_file->SEnd;
    if (data != NULL)
    {
        (*data) = NULL;
    }
    data = load_file->Start;
    if (data != NULL) {
        if ((*data) != NULL) {
            LOGSYNC("%s: freeing ...", load_file->FName);
            LbMemoryFree(*data);
            (*data) = NULL;
            LOGSYNC("done");
        }
    }
    return 1;
}

short LbDataFreeAll(struct TbLoadFiles load_files[])
{
    struct TbLoadFiles *t_lfile;
    t_lfile = &load_files[0];
    // note that t_lfile->Start is not NULL even if the buffer is allocated
    while (t_lfile->Start != NULL) {
        LbDataFree(t_lfile);
        t_lfile++;
    }
    return 1;
}

short LbDataLoad(struct TbLoadFiles *load_file)
{
    LbMemorySetup();
    MemAllocFunc alloc_func;
    if (load_file->Flags & 0x0001)
      alloc_func = &LbMemoryAllocLow;
    else
      alloc_func = &LbMemoryAlloc;
    LbDataFree(load_file);
    char *fname = modify_data_load_filename_function(load_file);
    if (fname[0] == '*')
    {
        LOGSYNC("%s: * in fname", fname);
        *(load_file->Start) = alloc_func(load_file->SLength);
        if ( (*(load_file->Start)) == NULL )
            return -100;
    } else
    {
        long slength = LbFileLengthRnc(fname);
        LOGDBG("%s: filelength %ld", fname, slength);
        load_file->SLength = slength;
        if (slength <= 0)
            return -101;
        *(load_file->Start) = alloc_func(slength + 512);
        if ((*(load_file->Start)) == NULL)
            return -100;
        if (LbFileLoadAt(fname, *(load_file->Start)) != (long)load_file->SLength)
        {
            *(load_file->Start) = 0;
            if (load_file->SEnd != NULL)
              *(load_file->SEnd) = 0;
            load_file->SLength = 0;
            return -101;
        }
    }
    if (load_file->SEnd != NULL)
      *(load_file->SEnd) = *(load_file->Start) + load_file->SLength;
    return 1;
}

short LbDataLoadAll(struct TbLoadFiles load_files[])
{
    struct TbLoadFiles *t_lfile;
    int ferror;
    int ret_val;
    int i;
    LbMemorySetup();
    LbDataFreeAll(load_files);
    ferror = 0;
    i = 0;
    t_lfile = &load_files[i];
    while (t_lfile->Start != NULL)
    {
        ret_val = LbDataLoad(t_lfile);
        if ( ret_val == -100 ) {
            LOGERR("%s: cannot allocate memory", t_lfile->FName);
          ferror++;
        }
        else if ( ret_val == -101 ) {
            LOGERR("%s: cannot load file", t_lfile->FName);
          ferror++;
        }
        i++;
        t_lfile = &load_files[i];
    }
    return ferror;
}

ModifyDataLoadFnameFunc LbDataLoadSetModifyFilenameFunction(ModifyDataLoadFnameFunc newfunc)
{
    modify_data_load_filename_function = newfunc;
    return newfunc;
}

char *defaultModifyDataLoadFilename(struct TbLoadFiles *ldfiles)
{
    return ldfiles->FName;
}

int LbDataFindNameIndex(struct TbLoadFiles load_files[],char *fname)
{
    struct TbLoadFiles *t_lfile;
    int i;
    i = 0;
    t_lfile = &load_files[i];
    while (t_lfile->Start != NULL)
    {
        if (strcasecmp(t_lfile->FName,fname) == 0)
            return i;
        i++;
        t_lfile = &load_files[i];
    }
    return -1;
}

int LbDataFindStartIndex(struct TbLoadFiles load_files[],unsigned char **start)
{
    struct TbLoadFiles *t_lfile;
    int i;
    i = 0;
    t_lfile = &load_files[i];
    while (t_lfile->Start != NULL)
    {
        if (*t_lfile->Start == *start)
            return i;
        i++;
        t_lfile = &load_files[i];
    }
    return -1;
}
/******************************************************************************/
