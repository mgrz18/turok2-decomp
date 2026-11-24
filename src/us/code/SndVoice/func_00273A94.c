/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025B758_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/audio.h"

#ifndef TARGET_PORT
void SndVoicePool_Construct(s32 *arg0, sptr arg1) {
    s32 i;
    s32 idx;
    f32 t;
    f32 p;
    s32 off;
    RecordCode8025A3EC *rec;
    f32 k1;
    f32 k2;
    f32 k3;
    s32 negone;
    s32 one;
    i = 0;
    k1 = 0.011111111f;
    k2 = 3.2e4f;
    *arg0 = arg1;
    do {
        t = (f32)i * k1;
        p = t * t * t * t * t * k2;
        idx = 0x5A - i;
        i += 1;
        gSndVolumeCurve[idx] = (s16)(s32)p;
    } while (i < 0x5B);
    negone = -1;
    i = 0;
    k3 = 1.0f;
    one = 1;
    off = i;
    do {
        rec = (RecordCode8025A3EC *)((u32)off + (uptr)arg0);
        rec = &((ObjRecAt4V6 *)(rec))->unk4;
        rec->index = i;
        i += 1;
        rec->owner = arg1;
        rec->f0C = negone;
        rec->f08 = negone;
        rec->f3A = (s16)negone;
        rec->f38 = 0;
        rec->f40 = negone;
        rec->f14 = 0;
        rec->f2C = k3;
        rec->f58 = 0;
        rec->f5C = 0;
        rec->f34 = k3;
        rec->fA4 = 0;
        rec->fAC = 0;
        rec->fB4 = negone;
        rec->fB8 = k3;
        rec->fBC = 0;
        rec->fC0 = 0;
        rec->fC4 = one;
        off += 0xCC;
    } while (i < 0x11);
}
#else
#include "structs/sound.h"
/* The port: the same on the typed pool (the Rage Wars view above writes the 17 voices at N64
 * offsets, 0xCC apart): the volume curve, then every voice reset. */
void SndVoicePool_Construct(SndVoicePool *pool, CAudioWorld *m) {
    s32 i;
    pool->owner = m;
    for (i = 0; i < 0x5B; i++) {
        f32 t = (f32)i * 0.011111111f;
        gSndVolumeCurve[0x5A - i] = (s16)(s32)(t * t * t * t * t * 3.2e4f);
    }
    for (i = 0; i < 0x11; i++) {
        SndVoice *v = &pool->voices[i];
        v->index = i;
        v->mgr = m;
        v->id = -1;
        v->unk8 = -1;
        v->vp.channel = -1;
        v->vp.priority = 0;
        v->vp.unk30 = -1;
        v->vp.w4 = 0;
        v->vp.scale = 1.0f;
        v->vp.distSq = 0.0f;
        v->vp.prevDistSq = 0.0f;
        v->vp.doppler = 1.0f;
        v->flags = 0;
        v->stopping = 0;
        v->unkB4 = -1;
        v->pitchScale = 1.0f;
        v->hasTrack = 0;
        v->reset = 0;
        v->unkC4 = 1;
    }
}
#endif
