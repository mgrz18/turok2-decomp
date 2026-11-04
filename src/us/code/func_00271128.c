#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025828C.c, func_0025828C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/sound.h"
u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
static __inline__ void SndMgr_Lock(SndMgr *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();
    if (++l->count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&l->queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
}
static __inline__ void SndMgr_Unlock(SndMgr *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();
    if (--l->count != 0) {
        __osRestoreInt(mask);
        osSendMesg(&l->queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
}
void *func_002017D4(void *node, s32 idx);
void func_00270A60(SndMgr *m, void *rec, CVector3 *pos, s32 id, s32 token, s32 a, s32 flags);
s32 func_00271128(SndMgr *m, s32 id, CVector3 pos, s32 flags) {
    u32 *rec;
    u32 count;
    u32 i;
    s32 token;
    void *n;
    if (!(id < m->rangeCount)) {
        return -1;
    }
    n = func_002017D4(m->variants, (s16)id);
    n = func_002017D4(n, 0);
    m->cue = n;
    m->cueId = 0x100;
    m->cueArg = -1;
    n = func_002017D4(m->variants, (s16)id);
    rec = func_002017D4(n, 1);
    count = rec[1];
    if (count != 0) {
        SndMgr_Lock(m);
        token = ++m->handle;
        SndMgr_Unlock(m);
    } else {
        token = -1;
    }
    i = 0;
    if (count != 0) {
        do {
            func_00270A60(m, (u8 *)rec + (i * rec[0] + 8), &pos, id, token, 0, flags);
            i++;
        } while (i < count);
    }
    return token;
}
