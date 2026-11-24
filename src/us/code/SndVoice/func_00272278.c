#include "structs/vector.h"
#include "structs/sound.h"
#include "structs/camera.h"
#include "globals/audio.h"
#include "protos/half.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00259F88.c, func_00259F88.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

s32 Cinema_PathPlaying();
SndEnv *CAudioWorld__GetVolumeEnvelope(CAudioWorld *c, s16 id);
f32 Random_Float(f32 x);
void SndVoice_InitVolume(SndVoice *o, SoundEffectInfo *rec) {
    SndVoiceParams *vp = &o->vp;
    SndEnv *env;
    u8 *pp;
    s32 i;
    s32 kind;
    s32 n;
    s32 t;
    f32 vf;
    f32 a;
    f32 b;
    f32 f;
    f32 fs;
    CVector3 *p;
    CVector3 *q;
    pp = o->mgr->cue;
    o->volumeScale = 1.0f;
    if (o->soundId < 0x100) {
        for (i = 0; i < 0x1F; i++) {
            if (o->soundId == (s32)sSoundKindVolumeTable[i].kind) {
                o->volumeScale = sSoundKindVolumeTable[i].scale;
                break;
            }
        }
    }
    if (Cinema_PathPlaying() != 0) {
        o->volumeScale = o->volumeScale * 0.8f;
    }
    env = &o->env2;
    if (rec->unk8 != -1) {
        *env = *CAudioWorld__GetVolumeEnvelope(o->mgr, rec->unk8);
    } else {
        env->level = FloatToHalf((f32)rec->unkF);
    }
    if (!(rec->flags & 0x800) && o->reset != 1 && o->mgr->listener != 0) {
        a = (f32)pp[0] * 0.01f;
        b = (f32)pp[1] * 0.01f;
        if (a == 1.0f) {
            vf = 1.0f;
        } else if (b == a) {
            vf = a;
        } else {
            vf = a + Random_Float(b - a);
        }
        p = &vp->pos;
        q = &o->mgr->listener->m_vPos;
        {
            f32 d2 = (p->x - q->x) * (p->x - q->x) + (p->y - q->y) * (p->y - q->y) + (p->z - q->z) * (p->z - q->z);
            if (sSoundHearingRangeSq <= d2) {
                f = 0.0f;
            } else {
                f = 1.0f - d2 / sSoundHearingRangeSq;
            }
        }
        switch (vp->distanceMode) {
        case 2:
            vp->scale = f;
            break;
        case 0:
            f = f * f;
            f = f * f;
        case 1:
            f = f * f;
            vp->scale = f;
            break;
        default:
            f = 0.0f;
            vp->scale = 0.0f;
            break;
        }
        vp->f20 = f;
    } else {
        vf = 1.0f;
        vp->scale = vf;
        vp->f20 = vf;
    }
    if (rec->flags & 4) {
        vp->vol = (s32)HalfToFloat(env->level);
        fs = HalfToFloat(env->unk2);
        fs -= HalfToFloat(env->target);
        if (env->frames == 0) {
            o->env2Step = 0.0f;
        } else {
            o->env2Step = fs / (f32)env->frames;
        }
        o->env2Value = HalfToFloat(env->unk2);
        n = (s32)HalfToFloat(env->level);
        if ((f32)n < 0.0f) {
            t = -n * 2;
        } else {
            t = n * 2;
        }
        vp->vol = (s32)((f32)(s32)((f32)*(s16 *)((u8 *)gSndVolumeCurve + t) * o->env2Value) * vf);
        if ((f32)n < 0.0f) {
            t = -n * 2;
        } else {
            t = n * 2;
        }
        env->level = FloatToHalf((f32)*(s16 *)((u8 *)gSndVolumeCurve + t) * vf);
        vp->wC = (s32)HalfToFloat(env->level & 0xFFFF);
        return;
    }
    n = (s32)HalfToFloat(env->level);
    if ((f32)n < 0.0f) {
        t = -n * 2;
    } else {
        t = n * 2;
    }
    t = (s32)((f32)*(s16 *)((u8 *)gSndVolumeCurve + t) * vf);
    vp->vol = t;
    vp->wC = t;
}
