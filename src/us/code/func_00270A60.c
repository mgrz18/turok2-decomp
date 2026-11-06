#include "structs/vector.h"
#include "globals/camera.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00257BD4.c, func_00257BD4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/sound.h"
#include "structs/camera.h"
u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
/* Static inlines from the Rage Wars header b13_types.h (dropped by the importer). */
static inline void CAudioWorld_Lock(CAudioWorld *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();
    if (++l->count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&l->queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
}
static inline void CAudioWorld_Unlock(CAudioWorld *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();
    if (--l->count != 0) {
        __osRestoreInt(mask);
        osSendMesg(&l->queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
}
s32 CCameraPool__GetNearestCamera(void *p, s32 id);
s32 SndVoicePool_IsSoundPlaying(void *pool, s16 a);
f32 Sound_DistanceFalloff(s32 id, s32 p);
s32 SndVoicePool_FindSoundWithOwner(void *pool, s16 a, s32 b);
s32 SndQueue_Add(void *q, SoundEffectInfo *info, s32 id, s32 a3, s32 a5);
void SndVoicePool_StopGroupVoice(void *pool, s16 a, s16 b);
SndVoice *SndVoicePool_AllocVoice(void *pool, SoundEffectInfo *info, s32 a3, s32 a4);
void SndVoice_SetPosition(SndVoice *v, s32 id, s32 a6);
void SndVoice_SetSoundId(SndVoice *v, s16 a);
void SndVoice_Setup(SndVoice *v, SoundEffectInfo *info, s32 a5);
s32 CAudioWorld__StartVoice(CAudioWorld *m, s32 id, s32 a2, s32 a3);
s32 CAudioWorld__PlayLayer(CAudioWorld *m, SoundEffectInfo *info, s32 id, s32 a3, s32 a4, s32 a5, s32 a6) {
    SndVoice *v;
    m->listener = CCameraPool__GetNearestCamera(&gEngineApp_m_Game_m_CameraPool, id);
    if (info->flags & 1) {
        if (SndVoicePool_IsSoundPlaying(&m->pool, m->cueId) != 0) {
            return -1;
        }
        if (Sound_DistanceFalloff(id, (s32)&m->listener->m_vPos) == 0.0f) {
            return -1;
        }
    }
    if (info->flags & 0x1000) {
        if (m->cueArg != -1) {
            if (SndVoicePool_FindSoundWithOwner(&m->pool, m->cueId, m->cueArg) != -1) {
                return -1;
            }
        }
    }
    if (info->sample != 0) {
        return SndQueue_Add(&m->queue, info, id, a3, a5);
    }
    if (info->flags & 0x40) {
        SndVoicePool_StopGroupVoice(&m->pool, m->cueId, info->group);
    }
    v = SndVoicePool_AllocVoice(&m->pool, info, a3, a4);
    if (v == 0) {
        return -1;
    }
    SndVoice_SetPosition(v, id, a6);
    SndVoice_SetSoundId(v, m->cueId);
    SndVoice_Setup(v, info, a5);
    CAudioWorld_Lock(m);
    CAudioWorld__StartVoice(m, v->id, 0, id);
    CAudioWorld_Unlock(m);
    return 0;
}
