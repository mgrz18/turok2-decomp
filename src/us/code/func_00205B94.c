/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCENE/802508E0_11.CPP, func_80254284_de, with Turok 2's layouts. */

#include "common.h"
#include "ultra_types.h"
#include "globals/cache.h"

extern char D_800A51E0[];

u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
void **CCache__Request(s32, s32, s32, s32, s32, s32, void *, s32);
void CCache__DeallocateInternal(s32, void **);

/* Loads a four-byte probe to read a count, frees it under the resource lock, then loads count*4+15
 * rounded to eight bytes and stores its first word through arg1, returning the allocation. */
void **CCache__LoadPersistantIndex(s32 arg0, void **arg1, s32 arg2, void *arg3) {
    void **resource;
    s32 size;
    s32 counter;
    s32 counter2;
    u32 token;
    u32 token2;
    s32 one = 1;

    resource = CCache__Request(0, arg2, 4, 0x1B, 0, 0, D_800A51E0, one);
    size = **(s32 **)resource;
    token = __osDisableInt();
    counter = gCache_mutex_count + one;
    gCache_mutex_count = counter;
    if (counter != one) {
        __osRestoreInt(token);
        osRecvMesg(&gCache_mutex, 0, one);
    } else {
        __osRestoreInt(token);
    }
    CCache__DeallocateInternal(0, resource);
    token2 = __osDisableInt();
    counter2 = gCache_mutex_count - 1;
    gCache_mutex_count = counter2;
    if (counter2 != 0) {
        __osRestoreInt(token2);
        osSendMesg(&gCache_mutex, 0, 1);
    } else {
        __osRestoreInt(token2);
    }
    resource = CCache__Request(0, arg2, ((size * 4) + 0xF) & ~7, 0x1B, 0, 0, arg3, 1);
    *arg1 = *resource;
    return resource;
}
