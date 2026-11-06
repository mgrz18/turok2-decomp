#include "common.h"
#include "structs/sound.h"
#include "structs/camera.h"

extern f32 D_800B2848; /* volume step */

void SndVoice_UpdateDoppler(SndVoice *s) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 d;
    f32 v;
    f32 vol;
    CCamera *t;

    if (s->soundId != 0x1B2 && s->soundId != 0x1B3) {
        s->vp.doppler = 1.0f;
        return;
    }
    if (s->reset != 0) {
        s->vp.doppler = 1.0f;
    }
    t = s->mgr->listener;
    dx = s->vp.pos.x - t->m_vPos.x;
    dy = s->vp.pos.y - t->m_vPos.y;
    dz = s->vp.pos.z - t->m_vPos.z;
    d = dx * dx + dy * dy + dz * dz;
    s->vp.distSq = d;
    if (s->vp.prevDistSq < d) {
        s->vp.doppler -= D_800B2848;
    } else if (d < s->vp.prevDistSq) {
        s->vp.doppler += D_800B2848;
    } else {
        v = s->vp.doppler;
        if (v > 1.0f) {
            vol = v - D_800B2848;
            if (vol < 1.0f) {
                vol = 1.0f;
            }
            s->vp.doppler = vol;
        } else if (v < 1.0f) {
            vol = v + D_800B2848;
            if (vol > 1.0f) {
                vol = 1.0f;
            }
            s->vp.doppler = vol;
        }
        return;
    }
    if (s->vp.doppler > 1.4f) {
        s->vp.doppler = 1.4f;
    } else if (s->vp.doppler < 0.6f) {
        s->vp.doppler = 0.6f;
    }
    s->vp.prevDistSq = s->vp.distSq;
}
