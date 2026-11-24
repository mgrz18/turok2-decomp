#include "structs/vector.h"
#include "globals/audio.h"
#include "structs/sound.h"
#include "structs/camera.h"

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AUDIO/8025A3EC_4.CPP, func_8025C368_de, with Turok 2's layouts
 * (round 29, part r2: the sound is a SndVoiceParams, the camera a CCamera). */


/* Volume attenuation of a sound by its squared distance to the camera. */
f32 Sound_DistanceAttenuation(SndVoiceParams *sound, CCamera *camera) {
    CVector3 *a = &sound->pos;
    CVector3 *b = &camera->m_vPos;
    f32 dx = a->x - b->x;
    f32 dy;
    f32 dz;
    f32 d;
    f32 t;

    dx *= dx;
    dy = a->y - b->y;
    dy *= dy;
    dz = a->z - b->z;
    dz *= dz;
    d = dx + dy + dz;
    if (d >= sSoundHearingRangeSq) {
        t = 0.0f;
    } else {
        t = 1.0f - d / sSoundHearingRangeSq;
    }
    switch (sound->distanceMode) {
    case 2:
        return t;
    case 0:
        t *= t;
        t *= t;
    case 1:
        return t * t;
    default:
        return 0.0f;
    }
}
