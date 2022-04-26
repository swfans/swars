/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfsemphr.hpp
 *     Header file for ssemphr.cpp.
 * @par Purpose:
 *     Semaphores wrapper.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     21 May 2009 - 20 Jul 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIBRARY_BFSEMPHR_H_
#define BFLIBRARY_BFSEMPHR_H_

/******************************************************************************/

class LbSemaphore {
public:
    LbSemaphore(void);
    virtual ~LbSemaphore(void);
    union {
    void *pHandle;
    int iHandle;
    };
};

/******************************************************************************/

class LbSemaLock {
public:
    LbSemaLock(class LbSemaphore *sem, bool isLocked);
    virtual ~LbSemaLock(void);
    int Lock(bool wait_forever);
    void Release(void);
    union {
    void *pHandle;
    int iHandle;
    };
    bool locked:1;
    bool invalid:1;
};

/******************************************************************************/

#endif // BFLIBRARY_BFSEMPHR_H_
