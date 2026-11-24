/* libultra's interrupt mask pair (PR/os_internal_reg.h: OSIntMask is a u32), for the game code that
 * takes its own lock around a CMutex (CCache's gCache_mutex). Not in protos/ultra.h: files of other
 * modules still declare it with an s32 mask (round 28, part q2). */

#ifndef PROTOS_OSINT_H
#define PROTOS_OSINT_H

#include "common.h"

u32 __osDisableInt(void);
void __osRestoreInt(u32);

#endif
