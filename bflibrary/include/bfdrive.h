/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfdrive.h
 *     Header file for sdrive.c.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 25 Apr 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_BFDRIVE_H_
#define BFLIBRARY_BFDRIVE_H_

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

struct TbDriveInfo {
    ulong TotalClusters;
    ulong FreeClusters;
    ulong SectorsPerCluster;
    ulong BytesPerSector;
};

typedef struct TbDriveInfo TbDriveInfo;

#pragma pack()

/** Sets given pointer to the current drive number.
 *
 * @param drive
 * @return
 */
TbResult LbDriveCurrent(ulong *drive);

/** Changes the current disk drive into given one.
 *
 * @param drive
 * @return
 */
TbResult LbDriveChange(const ulong drive);

/** Returns if a given drive exists.
 *
 * @param drive
 * @return
 */
TbResult LbDriveExists(const ulong drive);

/** Acquires info on the drive geometry and free clusters.
 *
 * @param drive
 * @return
 */
TbResult LbDriveFreeSpace(const ulong drive, TbDriveInfo *drvinfo);

#ifdef __cplusplus
};
#endif

#endif // BFLIBRARY_BFDRIVE_H_
/******************************************************************************/
