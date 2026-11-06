/* WRONG CUT: REAL Pak_ProbeRumble size 0x74 (splat cut it into Pak_ProbeRumble + func_00426C74). */
#include "common.h"
#include "structs/pak.h"
#include "structs/controller.h"
#include "globals/pak.h"

extern void **gPakStateBuffer;
extern s32 gPakDirectoryAllocated;
extern u8 gPakQueueState;

extern void **CCache__Allocate(s32, s32, s32, char *);
extern void CCache__Flush(s32);
extern void CCache__Deallocate(s32, void **);
extern void Controller_Lock(s32);
extern void Controller_ReadAll(void);
extern void Controller_Unlock(void);
extern void *func_0029E460(void *dst, const void *src, s32 size);
extern s32 Pak_Mount(s32 ch);
extern s32 osPfsReadWriteFile(OSPfs *pfs, s32 file_no, u8 flag, s32 offset, s32 size, void *data);
extern u8 gSiMessageQueue[];
extern s32 osMotorInit(void *queue, void *pfs, s32 channel);

/* Probes slot ch for a Rumble Pak; returns whether one answered. */
s32 Pak_ProbeRumble(s32 ch) {
    RumblePak *cur;
    s32 ready;

    cur = &gRumblePaks[ch];
    if (gContStatus[ch].errno == 0) {
        cur->present = osMotorInit(gSiMessageQueue, &cur->pfs, ch) == 0;
        ready = cur->present;
    } else {
        cur->present = 0;
        ready = 0;
    }
    return ready;
}
