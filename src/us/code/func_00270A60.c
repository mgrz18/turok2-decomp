/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00257BD4.c, func_00257BD4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/sound.h"
u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
/* Static inlines from the Rage Wars header b13_types.h (dropped by the importer). */
static inline void SndMgr_Lock(SndMgr *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();
    if (++l->count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&l->queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
}
static inline void SndMgr_Unlock(SndMgr *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();
    if (--l->count != 0) {
        __osRestoreInt(mask);
        osSendMesg(&l->queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
}
extern u8 D_801198B0[];
s32 func_00281444(void *p, s32 id);
s32 func_00273D88(void *pool, s16 a);
f32 func_002743D0(s32 id, s32 p);
s32 func_002740E8(void *pool, s16 a, s32 b);
s32 func_00275FA0(void *q, SoundEffectInfo *info, s32 id, s32 a3, s32 a5);
void func_00273C5C(void *pool, s16 a, s16 b);
SndVoice *func_00273504(void *pool, SoundEffectInfo *info, s32 a3, s32 a4);
void func_00273E98(SndVoice *v, s32 id, s32 a6);
void func_00273EB8(SndVoice *v, s16 a);
void func_00273EC0(SndVoice *v, SoundEffectInfo *info, s32 a5);
s32 func_002706A0(SndMgr *m, s32 id, s32 a2, s32 a3);
s32 func_00270A60(SndMgr *m, SoundEffectInfo *info, s32 id, s32 a3, s32 a4, s32 a5, s32 a6) {
    SndVoice *v;
    m->listener = func_00281444(D_801198B0, id);
    if (info->flags & 1) {
        if (func_00273D88(&m->pool, m->cueId) != 0) {
            return -1;
        }
        if (func_002743D0(id, m->listener + 0x114) == 0.0f) {
            return -1;
        }
    }
    if (info->flags & 0x1000) {
        if (m->cueArg != -1) {
            if (func_002740E8(&m->pool, m->cueId, m->cueArg) != -1) {
                return -1;
            }
        }
    }
    if (info->sample != 0) {
        return func_00275FA0(&m->queue, info, id, a3, a5);
    }
    if (info->flags & 0x40) {
        func_00273C5C(&m->pool, m->cueId, info->group);
    }
    v = func_00273504(&m->pool, info, a3, a4);
    if (v == 0) {
        return -1;
    }
    func_00273E98(v, id, a6);
    func_00273EB8(v, m->cueId);
    func_00273EC0(v, info, a5);
    SndMgr_Lock(m);
    func_002706A0(m, v->id, 0, id);
    SndMgr_Unlock(m);
    return 0;
}
