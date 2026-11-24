/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO_2/802591C0.CPP, func_80258AE0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/vector.h"
#include "structs/sound.h"
#include "structs/camera.h"
#include "globals/audio.h"
#include "globals/camera.h"
#include "protos/audio_world.h"

s32 CAudioWorld__PlaySound(CAudioWorld *m, s32 id, CVector3 pos, sptr a5, s32 a6);

/* The game mode (gEngineApp_m_Settings_mode, 13: the ambient track plays) reached from the camera
 * pool's symbol, 0x13D8 past it: one %hi for both, as in the ROM (a call in Rage Wars). */
#define CAMERA_POOL_MODE (*(s32 *)((u8 *)&gEngineApp_m_Game_m_CameraPool + 0x13D8))

void CAudioWorld__UpdateAmbientTrack(CAudioWorld *m) {
    CVector3 zero;
    struct CCamera *camera;
    CVector3 *vec;
    if (gAudioWorldReady != 0 &&
        m->unk2BB4 != 0 &&
        CAMERA_POOL_MODE == 0xD &&
        m->track > 0) {
        {
            register f32 value = 0.0f;
            zero.z = value;
            zero.y = value;
            zero.x = value;
        }
        camera = CCameraPool__GetNearestCamera(&gEngineApp_m_Game_m_CameraPool, &zero);
        vec = &camera->m_vPos;
        if ((m->frame & 3) == 0) {
            CAudioWorld__PlaySound(m, m->track, *vec, 0, -1);
        }
    }
}
