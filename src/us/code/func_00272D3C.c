#include "common.h"
#include "structs/sound.h"
#include "structs/camera.h"

extern f32 D_800B2848; /* volume step */

void func_00272D3C(SndVoice *s) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 d;
    f32 v;
    f32 vol;
    CCamera *t;

    if (s->soundId != 0x1B2 && s->soundId != 0x1B3) {
        s->vp.f24 = 1.0f;
        return;
    }
    if (s->reset != 0) {
        s->vp.f24 = 1.0f;
    }
    t = s->mgr->listener;
    dx = s->vp.pos.x - t->m_vPos.x;
    dy = s->vp.pos.y - t->m_vPos.y;
    dz = s->vp.pos.z - t->m_vPos.z;
    d = dx * dx + dy * dy + dz * dz;
    s->vp.distSq = d;
    if (s->vp.prevDistSq < d) {
        s->vp.f24 -= D_800B2848;
    } else if (d < s->vp.prevDistSq) {
        s->vp.f24 += D_800B2848;
    } else {
        v = s->vp.f24;
        if (v > 1.0f) {
            vol = v - D_800B2848;
            if (vol < 1.0f) {
                vol = 1.0f;
            }
            s->vp.f24 = vol;
        } else if (v < 1.0f) {
            vol = v + D_800B2848;
            if (vol > 1.0f) {
                vol = 1.0f;
            }
            s->vp.f24 = vol;
        }
        return;
    }
    if (s->vp.f24 > 1.4f) {
        s->vp.f24 = 1.4f;
    } else if (s->vp.f24 < 0.6f) {
        s->vp.f24 = 0.6f;
    }
    s->vp.prevDistSq = s->vp.distSq;
}
