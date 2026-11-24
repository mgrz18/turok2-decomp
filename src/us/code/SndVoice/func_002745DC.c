#include "structs/vector.h"
#include "globals/audio.h"

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AUDIO/8025A3EC_4.CPP, func_8025C368_de, with Turok 2's layouts. */


typedef struct {
    char pad0[0x34];
    CVector3 position;          /* 0x34 */
    char pad40[0x4];
    s8 curve;                   /* 0x44 */
} SoundSpatialSource;

typedef struct {
    char pad0[0x114];
    CVector3 position;          /* 0x114 (0x128 in Rage Wars) */
} GameplayCamera;


/* Volume attenuation of a sound by its squared distance to the camera. */
f32 Sound_DistanceAttenuation(SoundSpatialSource *sound, GameplayCamera *camera) {
    CVector3 *a = &sound->position;
    CVector3 *b = &camera->position;
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
    switch (sound->curve) {
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
