/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/TEXTURE/80256220_16.CPP, func_8025842C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. Written over
 * structs/sound.h's CAudioWorld in round 28 (was a Rage Wars type dump). */

#include "common.h"
#include "ultra_types.h"
#include "structs/vector.h"
#include "structs/sound.h"
#include "structs/camera.h"
#include "globals/game.h"
#include "globals/camera.h"
#include "globals/audio.h"
#include "protos/ultra.h"
#include "protos/osint.h"
#include "protos/audio_world.h"

s32 CAudioWorld__PlaySound(CAudioWorld *m, s32 id, CVector3 pos, sptr a5, s32 a6);

/* Once a frame: the volume options (bytes 0x13-0x16 of the settings), the voices under the lock,
 * the queue, speech and music, and the ambient track (mode 13) from the camera nearest the origin. */
s32 CAudioWorld__Update(CAudioWorld *m) {
    u8 *bytes = &gEngineApp_m_Settings.unk15;
    CMutex *lock;
    u32 mask;
    s32 count;
    m->sfxVolume = (f32)bytes[0] * 0.003921569f;
    m->volume2BA4 = (f32)bytes[-1] * 0.003921569f;
    m->musicVolume = (f32)bytes[1] * 0.003921569f;
    lock = &m->lock;
    m->unk2BB0 = bytes[-2];
    mask = __osDisableInt();
    count = lock->count + 1;
    lock->count = count;
    if (count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&lock->queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    SndVoicePool_Update(&m->pool);
    {
        CMutex *lock2 = &m->lock;
        u32 mask2 = __osDisableInt();
        s32 count2 = lock2->count - 1;
        lock2->count = count2;
        if (count2 != 0) {
            __osRestoreInt(mask2);
            osSendMesg(&lock2->queue, 0, 1);
        } else {
            __osRestoreInt(mask2);
        }
    }
    SndQueue_Update(&m->queue);
    SpeechPlayer_Update(&m->fx);
    SndSeq_UpdateFade(&m->seq);
    SndSeq_Update(&m->seq);
    if (gAudioWorldReady != 0 &&
        m->unk2BB4 != 0 &&
        gEngineApp_m_Settings.mode == 13 &&
        m->track > 0) {
        CVector3 zero;
        struct CCamera *camera;
        CVector3 *vec;
        zero.z = 0.0f;
        zero.y = 0.0f;
        zero.x = 0.0f;
        camera = CCameraPool__GetNearestCamera(&gEngineApp_m_Game_m_CameraPool, &zero);
        vec = &camera->m_vPos;
        if ((m->frame & 3) == 0) {
            CAudioWorld__PlaySound(m, m->track, *vec, 0, -1);
        }
    }
    return ++m->frame;
}
