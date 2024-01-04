/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_test_math.c
 *     Test application for math functions.
 * @par Purpose:
 *     Testing implementation of bflibrary routines.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     25 May 2022 - 29 Jun 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <math.h>
#include <stdlib.h>
#include "bfmath.h"
#include "bfscreen.h"
#include "bfwindows.h"
#include "bftstlog.h"

/******************************************************************************/

TbResult MockZeroBaseInitialise(void)
{
    lbScreenInitialised = false;
    lbAppActive = true;
    lbLibInitialised = true;
    return Lb_SUCCESS;
}

/** Test square root computation by LbSqrL().
 */
TbBool test_sqrtl(void)
{
    ulong m;
    ulong x;

    if (MockZeroBaseInitialise() != Lb_SUCCESS) {
        LOGERR("bullfrog Library initialization failed");
        return false;
    }
    m = 1;
    x = 0;
    while (x+m > x) { /* repeat until we are at overflow */
        long sr1, sr2;
        sr1 = LbSqrL(x);
        sr2 = sqrt(x);

        if (sr1 != sr2) {
            LOGERR("LbSqrL(%ld) != %ld", sr1, sr2);
            return false;
        }

        /* at first, multiply the increment *2 */
        if ((x < m) && (m < pow(2,31))) {
            x += m;
            m *= 2;
        /* when reached 2^31, start dividing the increment */
        } else if (m > pow(2,8)) {
            x += m;
            m /= 2;
        /* when the increment falls too much, boost it up */
        } else {
            x += m;
            m *= 256;
        }
    }

    LOGSYNC("passed");
    return true;
}

int main(int argc, char *argv[])
{
    if (!test_sqrtl())
        exit(53);
    exit(0);
}

/******************************************************************************/
