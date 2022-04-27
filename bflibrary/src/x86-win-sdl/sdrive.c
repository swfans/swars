/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file sdrive.c
 *     Current drive handling routines wrapper.
 * @par Purpose:
 *     OS separation library for current drive handling routines.
 * @par Comment:
 *     Wraps drive handling functions, for OSes with separate drives access.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 25 Apr 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bfdrive.h"

#if defined(WIN32)||defined(DOS)||defined(GO32)
#include <direct.h>
#endif

TbResult LbDriveCurrent(ulong *drive)
{
#if defined(WIN32)||defined(DOS)||defined(GO32)
    *drive = _getdrive();
#else
    // Let's assume we're on 'C' drive on Unix ;)
    *drive = 3;
#endif
    return 1;
}

TbResult LbDriveChange(const ulong drive)
{
    int result;
#if defined(WIN32)||defined(DOS)||defined(GO32)
    int reterror = _chdrive(drive);
    if (reterror) {
        result = -1;
    } else {
        result = 1;
    }
#else
    // Let's assume we can only be on 'C' drive on Unix
    if (drive != 3) {
        result = -1;
    } else {
        result = 1;
    }
#endif
    return result;
}

TbResult LbDriveExists(const ulong drive)
{
    int result;
#if defined(WIN32)||defined(DOS)||defined(GO32)
    unsigned int lastdrive = _getdrive();
    if (_chdrive(drive)) {
        result = -1;
    } else {
        result = 1;
        _chdrive(lastdrive);
    }
#else
    // Let's assume we have only 'C' drive on Unix
    if (drive != 3) {
        result = -1;
    } else {
        result = 1;
    }
#endif
    return result;
}

TbResult LbDriveFreeSpace(const ulong drive, TbDriveInfo *drvinfo)
{
  int result;
#if defined(WIN32)||defined(DOS)||defined(GO32)
    struct _diskfree_t diskspace;
    int reterror = _getdiskfree(drive, &diskspace);
    if ( reterror )
    {
        result = -1;
    } else
    {
        drvinfo->TotalClusters = diskspace.total_clusters;
        drvinfo->FreeClusters = diskspace.avail_clusters;
        drvinfo->SectorsPerCluster = diskspace.sectors_per_cluster;
        drvinfo->BytesPerSector = diskspace.bytes_per_sector;
        result = 1;
    }
#else
    //On non-win32 systems - return anything big enough
    drvinfo->TotalClusters = 65535;
    drvinfo->FreeClusters = 65535;
    drvinfo->SectorsPerCluster = 512;
    drvinfo->BytesPerSector = 512;
    result = 1;
#endif
    return result;
}

/******************************************************************************/
