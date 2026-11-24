#include "structs/vector.h"
#include "structs/camera.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00259B30.c, func_00259B30.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

f32 func_002982F0(f32 x);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CMtxF__InverseRotateVector(CMtxF *m, CVector3 *a, CVector3 *out);
void CVector3__Normalize(CVector3 *v);
f32 Math_AcosLookup(f32 x);
f32 __builtin_fabsf(f32 x);

/* The pan (0..0x80) of a sound at pos heard from the camera (round 29, part r2: typed as the
 * port's copy in port/audio/t2audio_world.c). */
#ifndef TARGET_PORT /* the port's in port/ (port/cmake: Sound_CalcPan) */
s32 Sound_CalcPan(CVector3 *pos, CCamera *cam) {
    CVector3 r;
    CVector3 d;
    CVector3 w;
    f32 s;
    f32 zero;
    if (cam == NULL) {
        return 0x40;
    }
    CVector3__Subtract(&d, &cam->m_vPos, pos);
    if (d.x * d.x + d.z * d.z < 1.0e2f) {
        return 0x40;
    }
    CMtxF__InverseRotateVector(&cam->m_mfViewOrient, &d, &w);
    w.y = 0.0f;
    zero = w.y;
    r.y = zero;
    r.x = zero;
    r.z = 1.0f;
    CVector3__Normalize(&w);
    s = func_002982F0(Math_AcosLookup(w.x * r.x + w.y * r.y + w.z * r.z));
    if (w.x == zero) {
        return 0x40;
    }
    if (zero < w.x) {
        if (s < zero) {
            s = -s;
        }
    } else {
        s = -__builtin_fabsf(s);
    }
    return (s16)(s32)(s * 64.0f + 64.0f);
}
#endif

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_13.c, func_80259C5C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. Over
 * structs/sound.h (round 29, part r2: Rage Wars' ViewRecAtB0 is the SndVoice, its "angles" the
 * pitch envelope env, its "motion" vp.pitch/vp.distSq; ControllerCode802591C0 is SoundEffectInfo). */

#include "common.h"
#include "structs/sound.h"
#include "globals/audio.h"
#include "protos/half.h"

extern f32 Random_Blend(f32 arg0, f32 arg1);
SndEnv *CAudioWorld__GetPitchEnvelope(CAudioWorld *m, s32 id);
extern s16 SoundEffectInfo_GetRandomRange0(SoundEffectInfo *); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */
extern s16 SoundEffectInfo_GetRandomRange1(SoundEffectInfo *); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoice_InitPitch) */
void SndVoice_InitPitch(SndVoice *v, SoundEffectInfo *info, s32 flags) {
    s16 x;
    s16 y;
    f32 fx;
    f32 fy;
    s32 turn;
    f32 diff;
    f32 position;
    s16 index;
    s16 next;
    f32 base;
    f32 result;
    SndVoiceParams *vp;
    SndEnv *env;
    vp = &v->vp;
    env = &v->env;
    vp->distSq = vp->prevDistSq = 0.0f;
    if (info->unkA != -1) {
        *env = *CAudioWorld__GetPitchEnvelope(v->mgr, info->unkA);
    } else {
        env->level = FloatToHalf(info->unk10);
    }
    x = SoundEffectInfo_GetRandomRange0(info);
    y = SoundEffectInfo_GetRandomRange1(info);
    fy = y;
    fx = x;
    turn = (s16)(s32)(fx + ((Random_Blend(fy, fx)) < 0.0f ? -(Random_Blend(fy, fx)) : (Random_Blend(fy, fx))));
    env->level = FloatToHalf(HalfToFloat(env->level) + turn);
    if (flags & 0x40) {
        env->level = FloatToHalf(HalfToFloat(env->level) + 1.0e3f);
    }
    if (flags & 0x80) {
        env->level = FloatToHalf(HalfToFloat(env->level) - 1.0e3f);
    }
    if (HalfToFloat(env->level) >= 1.2e3f) {
        env->level = FloatToHalf(1.2e3f);
    }
    if (info->flags & 2) {
        vp->pitch.f = HalfToFloat(env->level);
        diff = HalfToFloat(env->unk2);
        diff -= HalfToFloat(env->target);
        if (env->frames == 0) {
            v->envStep = 0.0f;
        } else {
            v->envStep = diff / env->frames;
        }
        v->envValue = HalfToFloat(env->unk2);
    }
    position = (HalfToFloat(env->level) + 6.0e3f) * 0.016666668f;
    index = position;
    if (index != position) {
        next = index + 1;
        base = sPitchRatioTable[index];
        result = base + (position - index) * (sPitchRatioTable[next] - base);
    } else {
        result = sPitchRatioTable[index];
    }
    vp->pitch.f = result;
    env->level = FloatToHalf(result);
}
#endif

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00259F88.c, func_00259F88.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

s32 Cinema_PathPlaying();
SndEnv *CAudioWorld__GetVolumeEnvelope(CAudioWorld *c, s16 id);
f32 Random_Float(f32 x);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoice_InitVolume) */
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
#endif

#include "protos/sndp.h"

s32 Cinema_IsActive(void);
f32 Math_Approach(f32 value, f32 target, f32 rate);
#define MAX(a, b) ((a) < (b) ? (b) : (a))

void SndVoice_UpdateVolume(SndVoice *o) {
    SndVoiceParams *vp;
    SndEnv *env;
    CAudioWorld *settings;
    CAudioWorld *ctx;
    f32 hear;
    f32 range;
    f32 rangeSq;
    f32 master;
    f32 d2;
    f32 f;
    f32 g;
    f32 vol;
    f32 gain;
    s32 down;
    s32 level;
    s32 vi;
    s32 slot;
    void *sndp;

    hear = o->mgr->listener->m_FarClip;
    range = MAX(hear, 3072.0f);
    rangeSq = range * MAX(hear, 3072.0f);
    sSoundHearingRangeSq = rangeSq;
    switch (o->vp.distanceMode) {
        case 2:
            sSoundHearingRangeSq = rangeSq + rangeSq * 0.5f;
            break;
        case 1:
            sSoundHearingRangeSq = rangeSq + rangeSq * 0.25f;
            break;
        case 0:
            break;
    }
    vp = &o->vp;
    env = &o->env2;

    settings = o->mgr;
    if (o->flags & 0x200) {
        master = settings->musicVolume;
    } else {
        master = settings->sfxVolume;
        if (o->unkC4 != 0 && settings->muted != 0) {
            master *= 0.5f;
        }
    }

    if ((o->flags & 0x400) && o->reset == 0 && !(o->flags & 0x800)) {
        {
            CVector3 *p = &vp->pos;
            CVector3 *q = &o->mgr->listener->m_vPos;
            d2 = (p->x - q->x) * (p->x - q->x) + (p->y - q->y) * (p->y - q->y) + (p->z - q->z) * (p->z - q->z);
        }
        if (sSoundHearingRangeSq <= d2) {
            f = 0.0f;
        } else {
            f = 1.0f - d2 / sSoundHearingRangeSq;
        }
        switch (vp->distanceMode) {
            case 2:
                g = f;
                break;
            case 0:
                f = f * f;
                f = f * f;
            case 1:
                g = f * f;
                break;
            default:
                g = 0.0f;
                break;
        }
        if (Cinema_IsActive() != 0) {
            vp->scale = Math_Approach(vp->scale, g, 0.017f);
        } else {
            vp->scale = g;
        }
    } else {
        vp->scale = 1.0f;
    }

    if (o->flags & 4) {
        if (o->env2.delay > 0) {
            o->env2.delay--;
        } else {
            down = 0;
            if (o->env2Step < 0.0f) {
                down = 1;
            }
            if (down ? (o->env2Value < HalfToFloat(env->target)) : (HalfToFloat(env->target) < o->env2Value)) {
                o->env2Value -= o->env2Step;
                if (down) {
                    if (HalfToFloat(env->target) < o->env2Value) {
                        o->env2Value = HalfToFloat(env->target);
                    }
                } else if (o->env2Value < HalfToFloat(env->target)) {
                    o->env2Value = HalfToFloat(env->target);
                    if (o->env2Value == 0.0f) {
                        ctx = o->mgr;
                        o->stopping = 1;
                        o->vp.follow = 0;
                        if (o->vp.frame != ctx->frame) {
                            alSndpSetSound(&ctx->player, ctx->handles[o->index]);
                            if (alSndpGetState(&ctx->player) != 0) {
                                alSndpStop(&ctx->player);
                            }
                            o->sound = -1;
                        }
                    }
                }
            }
        }
        level = HalfToFloat(o->env2.level) * o->env2Value;
        vol = (f32)level * vp->scale;
        vp->vol = level;
    } else {
        if (o->flags & 0x400) {
            gain = vp->scale;
        } else {
            gain = vp->f20;
        }
        vol = (f32)vp->vol * gain;
    }

    sndp = &o->mgr->player;
    slot = o->index;
    /* An empty statement macro (a compiled-out debug check, probably): the
     * scheduling block boundary it makes is what puts the multiply after the
     * loads. */
    do {
    } while (0);
    vi = vol * master;
    alSndpSetSound(sndp, o->mgr->handles[slot]);
    alSndpSetVol(sndp, (s32)((f32)vi * o->volumeScale));
}

/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0025A864.c, func_0025A864, with Turok 2's layouts. */

s32 CAudioWorld__GetStereo(CAudioWorld *c);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoice_UpdatePan) */
void SndVoice_UpdatePan(SndVoice *o) {
    SndVoiceParams *vp;
    s32 neg;
    u8 pan;
    ALSndPlayer *pl;
    CAudioWorld *c;

    vp = &o->vp;
    if (CAudioWorld__GetStereo(o->mgr) == 0) {
        c = o->mgr;
        alSndpSetSound(&c->player, c->handles[o->index]);
        alSndpSetPan(&c->player, 0x40);
        return;
    }
    if (o->hasEnv != 0) {
        if (o->env3.delay > 0) {
            o->env3.delay = o->env3.delay - 1;
        } else {
            neg = 0;
            if (o->env3Step < 0.0f) {
                neg = 1;
            }
            if (neg) {
                if (o->env3Value < HalfToFloat(o->env3.target)) {
                    goto slide;
                }
            } else if (HalfToFloat(o->env3.target) < o->env3Value) {
slide:
                o->env3Value = o->env3Value - o->env3Step;
                if (neg) {
                    if (HalfToFloat(o->env3.target) < o->env3Value) {
                        o->env3Value = HalfToFloat(o->env3.target);
                    }
                } else {
                    if (o->env3Value < HalfToFloat(o->env3.target)) {
                        o->env3Value = HalfToFloat(o->env3.target);
                    }
                }
            }
        }
        vp->pan = (s32)o->env3Value;
        goto setpan;
    }
    if ((o->flags & 0x400) && o->reset == 0) {
        vp->pan = Sound_CalcPan(&o->vp.pos, o->mgr->listener);
setpan:
        pan = ((u8 *)&vp->pan)[1];
        pl = &o->mgr->player;
        alSndpSetSound(pl, o->mgr->handles[o->index]);
        alSndpSetPan(pl, pan);
        return;
    }
    vp->pan = 0x40;
}
#endif

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
        s->vp.doppler -= sSndVoiceRampStep;
    } else if (d < s->vp.prevDistSq) {
        s->vp.doppler += sSndVoiceRampStep;
    } else {
        v = s->vp.doppler;
        if (v > 1.0f) {
            vol = v - sSndVoiceRampStep;
            if (vol < 1.0f) {
                vol = 1.0f;
            }
            s->vp.doppler = vol;
        } else if (v < 1.0f) {
            vol = v + sSndVoiceRampStep;
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

#define PITCH_TABLE sPitchRatioTable

static __inline__ f32 centsToPitch(f32 x) {
    f32 pos = (x + 6000.0f) * 0.016666668f;
    s16 i = pos;
    f32 fi = i;
    s16 j;
    f32 a;

    if (fi != pos) {
        j = i + 1;
        a = PITCH_TABLE[i];
        return a + (pos - fi) * (PITCH_TABLE[j] - a);
    }
    return PITCH_TABLE[i];
}

static __inline__ void setPitch(SndVoice *s, f32 pitch) {
    CAudioWorld *bank = s->mgr;

    alSndpSetSound(&bank->player, bank->handles[s->index]);
    alSndpSetPitch(&bank->player, pitch * s->vp.doppler * s->pitchScale);
}

void SndVoice_UpdatePitch(SndVoice *s) {
    SndVoiceParams *params;
    SndEnv *target;
    CAudioWorld *bank;
    s32 update;
    s32 down;
    f32 pitch;

    update = 0;
    params = &s->vp;
    target = &s->env;
    bank = s->mgr;
    if ((s->flags & 0x400) && !(bank->frame & 3)) {
        SndVoice_UpdateDoppler(s);
        update = 1;
    }
    if ((s->flags & 0xA) == 0xA) {
        if (s->env.delay > 0) {
            s->env.delay--;
        } else {
            down = 0;
            if (s->envStep < 0.0f) {
                down = 1;
            }
            if (down ? s->envValue < HalfToFloat(target->target) : HalfToFloat(target->target) < s->envValue) {
                s->envValue -= s->envStep;
                if (down) {
                    if (HalfToFloat(target->target) < s->envValue) {
                        s->envValue = HalfToFloat(target->target);
                    }
                } else {
                    if (s->envValue < HalfToFloat(target->target)) {
                        s->envValue = HalfToFloat(target->target);
                    }
                }
                pitch = centsToPitch(s->envValue);
                params->pitch.f = pitch;
                setPitch(s, pitch);
                return;
            }
        }
    }
    if (update) {
        setPitch(s, params->pitch.f);
    }
}

#include "globals/camera.h"

/* The bank seen from 0x7C: the ROM addresses its sound ids from there. */
typedef struct SoundVoice {
    /* 0x00 */ u8 pad0[0x60];
    /* 0x60 */ s16 ids[1];                     /* CAudioWorld.handles */
} SoundVoice;
extern sptr CCameraPool__GetNearestCamera(void *game, CVector3 *pos);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoice_Update) */
void SndVoice_Update(SndVoice *s) {
    CAudioWorld *bank;
    s32 playing;
    void *player;

    s->mgr->listener = CCameraPool__GetNearestCamera(&gEngineApp_m_Game_m_CameraPool, &s->vp.pos);
    bank = s->mgr;
    alSndpSetSound(&bank->player, bank->handles[s->index]);
    playing = alSndpGetState(&bank->player);
    if ((s->vp.scale <= 0.075f && s->soundId != 0x228) || ((s->flags & 4) && s->vp.vol == 0)) {
        s->stopping = 1;
    }
    if (s->hasTrack && s->mgr->track != s->soundId) {
        s->stopping = 1;
    }
    if (playing) {
        if (s->stopping) {
            CAudioWorld *b = s->mgr;
            s->stopping = 1;
            s->vp.follow = NULL;
            player = &b->player;
            if (s->vp.frame != b->frame) {
                alSndpSetSound(player, b->handles[s->index]);
                if (alSndpGetState(player)) {
                    alSndpStop(player);
                }
                s->sound = -1;
            }
            return;
        }
    } else if (s->delay > 0) {
        s->delay--;
        s->flags |= 0x10;
        return;
    }
    if ((s->flags & 0x18) == 0x10) {
        alSndpPlay(&s->mgr->player);
        return;
    }
    if (!playing) {
        CAudioWorld *b = s->mgr;
        SoundVoice *v = (SoundVoice *)((u8 *)b + 0x7C);
        alSndpDeallocate(&b->player, v->ids[s->index]);
        v->ids[s->index] = -1;
        s->vp.priority = 0;
        s->id = -1;
        s->unk8 = -1;
        s->soundId = -1;
        s->vp.channel = -1;
        s->unkB4 = -1;
        return;
    }
    if (s->vp.follow != NULL) {
        s->vp.pos = *s->vp.follow;
    }
    SndVoice_UpdateVolume(s);
    SndVoice_UpdatePan(s);
    SndVoice_UpdatePitch(s);
}
#endif

#include "rw/junk_types.h" /* SndCore */

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoicePool_FindSlot) */
s32 SndVoicePool_FindSlot(SndVoicePool *p, s32 a, s32 b, s16 pri, s32 c) {
    CAudioWorld *m;
    SndCore *core;
    s32 i;
    s32 best;
    s32 bestIdx;
    s16 lim;
    SndVoice *v;
    SndVoice *s;
    CAudioWorld *m2;
    s32 r;
    best = pri;
    bestIdx = -1;
    v = p->voices;
    lim = best;
    m = p->owner;
    core = (SndCore *)((u8 *)m + 0x7C);
    for (i = 0; i < 16; i++) {
        if (core->snd[i] == -1) {
            return (s16)i;
        }
        if (v->vp.priority < lim) {
            if (!(best < v->vp.priority) && v->vp.priority != 100) {
                best = v->vp.priority;
                bestIdx = i;
            }
        }
        v++;
    }
    if (bestIdx == -1) {
        return -1;
    }
    if (core->snd[bestIdx] == -1) {
        SndNode *h = (SndNode *)m->queue.active.link.next;
        if ((SndLink *)h == &m->queue.active.link) {
            return (s16)bestIdx;
        }
        if (h->body.vp.priority > pri) {
            return -1;
        }
        return (s16)bestIdx;
    }
    {
        (s) = (SndVoice *)(((s16)bestIdx) * 0xCC + (uptr)(p)); (s) = (SndVoice *)((u8 *)(s) + 4);
        m2 = s->mgr;
        s->stopping = 1;
        s->vp.follow = 0;
        if (s->vp.frame != m2->frame) {
            alSndpSetSound(&m2->player, m2->handles[s->index]);
            if (alSndpGetState(&m2->player) != 0) {
                alSndpStop(&m2->player);
            }
            s->sound = -1;
        }
        r = -1;
        if (p->voices[16].id != r) {
            return r;
        }
        return 16;
    }
}
#endif

void CAudioWorld__Lock(CAudioWorld *m);
void CAudioWorld__Unlock(CAudioWorld *m);
s16 SoundEffectInfo_GetGroup(SoundEffectInfo *i);
s8 SoundEffectInfo_GetPriority(SoundEffectInfo *i);
s8 SoundEffectInfo_GetChance(SoundEffectInfo *i);
s8 SoundEffectInfo_GetPan(SoundEffectInfo *i);
s16 SoundEffectInfo_GetFlags(SoundEffectInfo *i);
s32 Random_NextB();

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoicePool_AllocVoice) */
SndVoice *SndVoicePool_AllocVoice(SndVoicePool *p, SoundEffectInfo *info, int tag, s32 a3)
{
  s16 *snd;
  CAudioWorld *m3;
  CAudioWorld *m;
  s32 i;
  s16 idx;
  u8 *core;
  s32 pr;
  SndVoiceParams *vp;
  s32 pri;
  unsigned int sv;
  u16 flag;
  f32 f;
  u8 *new_var2;
  CAudioWorld *m2;
  s16 *new_var;
  s32 r;
  SndVoice *v;
  pri = SoundEffectInfo_GetGroup(info);
  flag = SoundEffectInfo_GetPriority(info);
  m = p->owner;
  snd = m->handles;
  if (((M04Bank2 *) m->bank)->tab->n < pri)
  {
    return 0;
  }
  CAudioWorld__Lock(m);
  new_var = &(*((s16 *) (((u8 *) info) + 2)));
  if ((SoundEffectInfo_GetChance(info) == 0x64) || ((Random_NextB() % 100) < SoundEffectInfo_GetChance(info)))
  {
    idx = ((s16 (*)(SndVoicePool *, s16, s16, s16, s32))SndVoicePool_FindSlot)(p, (s16) tag, pri, (s16) flag, SoundEffectInfo_GetFlags(info));
    if (idx != (-1))
    {
      v = (SndVoice *) (&p->voices[idx]);
      if (idx == 0x10)
      {
        v->sound = a3;
        v->id = p->owner->soundCounter++;
        v->unk8 = 0x63;
        goto blk16;
      }
      p->owner->pendingCue = idx;
      m2 = p->owner;
      new_var2 = &m2->player;
      core = ((u8 *) m2) + 0x7C;
      snd[idx] = ((s16 (*)(void *, void *, void *))alSndpAllocate)(new_var2, ((M04Bank2 *) m2->bank)->tab->v[pri], m2);
      sv = snd[idx];
      if (sv == (-1))
      {
        v->id = sv;
        v->unk8 = sv;
        v->vp.priority = 0;
        p->owner->pendingCue = sv;
        fail:
        CAudioWorld__Unlock(p->owner);
        return 0;
      }
      f = ((f32) ((s16) flag)) * 1.27f;
      (*((u16 *) (core + 0x80)))++;
      v->sound = a3;
      m3 = p->owner;
      v->id = m3->soundCounter++;
      v->unk8 = snd[idx];
      v->delay = *new_var;
      if (!(2.1474836e9f <= f))
      {
        pr = (s32) f;
      }
      else
      {
        pr = (s32) (f - 2.1474836e9f);
        pr = pr | 0x80000000;
      }
      alSndpSetPriority(new_var2, snd[idx], pr & 0xFF);
      blk16:
      v->soundId = p->owner->cueId;
      v->reset = p->owner->unk2BAC;
      v->hasTrack = 0;
      for (i = 0; i < 4; i++) { if (v->soundId == sVoiceTrackSounds[i]) { v->hasTrack = 1; break; } }
      v->unkC4 = 1;
      for (i = 0; i < 6; i++) { if (v->soundId == sVoiceNoUnkC4Sounds[i]) { v->unkC4 = 0; break; } }
      v->unkB4 = p->owner->cueArg;
      v->stopping = 0;
      vp = &v->vp;
      v->pitchScale = p->owner->unk2BBC;
      vp->priority = flag;
      vp->vol = (vp->wC = 0x7D00);
      vp->pitch.i = 0;
      vp->fxMix = *((s8 *) (((u8 *) info) + 0xE));
      vp->pan = 0x40;
      vp->channel = pri;
      vp->s2C = tag;
      vp->frame = p->owner->frame;
      vp->doppler = 1.0f;
      vp->distanceMode = SoundEffectInfo_GetPan(info);
      v->flags = SoundEffectInfo_GetFlags(info);
      vp->follow = 0;
      CAudioWorld__Unlock(p->owner);
      return v;
    }
  }
  goto fail;
}
#endif

/* FAKEMATCH (round 21 audit): an if/else whose two arms are the same code, kept
 * for the ROM's register allocation or layout; with one arm: SndVoicePool_StopAllExcept: 12 of 66 words.
 * */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025B5F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#ifndef TARGET_PORT
void SndVoicePool_StopAllExcept(RecordRecAt4V6 *record, s32 value)
{
  s32 i;
  s32 active;
  s32 absent;
  SlotRecAtB0 *slot = record->slots;
  i = 0;
  absent = -1;
  active = 1;
  for (; i < 16; slot++, i++)
  {
    if (((slot->used != absent) && (record->header->local != i)) && ((value == absent) || (slot->value != value)))
    {
      OwnerCode8025A3EC8025B5F0 *owner = slot->owner;
      void *sound;
      slot->active = active;
      slot->flag = 0;
      if (slot->key != owner->key)
      {
        sound = owner->sound;
        alSndpSetSound(sound, owner->samples[slot->index]);
        if (alSndpGetState(sound) != 0)
        {
          if (active)
          {
            alSndpStop(sound);
          }
          else
          {
            alSndpStop(sound);
          }
        }
        slot->state = absent;
      }
    }
  }
}
#else
#include "structs/sound.h"
/* The port: the same on the typed pool (the Rage Wars view above has N64 offsets: slot.used =
 * unk8, key = vp.frame, flag = vp.follow, value = soundId, active = stopping, owner = mgr;
 * header->local = pendingCue, owner->sound = player, samples = handles, owner->key = frame). */
void SndVoicePool_StopAllExcept(SndVoicePool *pool, s32 value) {
    s32 i;
    SndVoice *v = pool->voices;
    for (i = 0; i < 16; v++, i++) {
        if (v->unk8 != -1 && pool->owner->pendingCue != i && (value == -1 || v->soundId != value)) {
            CAudioWorld *m = v->mgr;
            v->stopping = 1;
            v->vp.follow = NULL;
            if (v->vp.frame != m->frame) {
                alSndpSetSound(&m->player, m->handles[v->index]);
                if (alSndpGetState(&m->player) != 0) {
                    alSndpStop(&m->player);
                }
                v->sound = -1;
            }
        }
    }
}
#endif

void SndVoice_Construct(SndVoice *arg0, CAudioWorld *arg1, s32 arg2) {
    arg0->id = -1;
    arg0->unk8 = -1;
    arg0->vp.channel = -1;
    arg0->vp.unk30 = -1;
    arg0->unkB4 = -1;
    arg0->mgr = arg1;
    arg0->index = arg2;
    arg0->vp.priority = 0;
    arg0->vp.w4 = 0;
    arg0->vp.distSq = 0;
    arg0->vp.prevDistSq = 0;
    arg0->flags = 0;
    arg0->stopping = 0;
    arg0->hasTrack = 0;
    arg0->reset = 0;
    arg0->unkC4 = 1;
    arg0->vp.scale = 1.0f;
    arg0->vp.doppler = 1.0f;
    arg0->pitchScale = 1.0f;
}

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

s32 SndVoicePool_GetVoiceState(s32 arg0, s16 arg1);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoicePool_GetVoiceState) */
s32 SndVoicePool_GetVoiceState(s32 arg0, s16 arg1)
{
  sptr temp_v0;
  sptr temp_v1;
  void *temp_s0;
  RecordCode8025A3EC *slot;
  OwnerCode8025A3EC8025B5F0 *owner;
  temp_v0 = (arg1 * 0xCC) + arg0;
  temp_v0 = temp_v0 + 4;
  slot = (RecordCode8025A3EC *)temp_v0;
  temp_v1 = slot->owner;
  owner = (OwnerCode8025A3EC8025B5F0 *)temp_v1;
  temp_s0 = owner->sound;
  alSndpSetSound(temp_s0, owner->samples[slot->index]);
  alSndpGetState(temp_s0);
}
#endif
void SndVoicePool_PlayVoice(SlotRecAtB8 *base, s16 index);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoicePool_PlayVoice) */
void SndVoicePool_PlayVoice(SlotRecAtB8 *base, s16 index)
{
  unsigned int offset = index * (sizeof(SlotRecAtB8));
  char *entry = (char *) offset;
  SlotRecAtB8 *slots;
  SlotRecAtB8 *selected;
  entry += (uptr) base;
  slots = base;
 do { alSndpPlay(0x84 + ((SlotRecAtB8 *) entry)->owner); entry = (char *) slots; entry += offset; selected = (SlotRecAtB8 *) entry; } while (0);
  selected->flags |= 8;
}
#endif

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoicePool_StopGroupVoice) */
s32 SndVoicePool_StopGroupVoice(SndVoicePool *p, s16 tag, s16 key) {
    s32 i;
    s32 j;
    SndVoice *v = p->voices;
    SndVoice *s;
    CAudioWorld *m;
    i = 0;
    for (; i < 16; i++) {
        if (v->unk8 != -1 && p->owner->pendingCue != i && v->soundId == tag && (v->flags & 0x40) != 0 && v->vp.channel == key) {
            j = (s16)i;
            (s) = (SndVoice *)((j) * 0xCC + (uptr)(p)); (s) = (SndVoice *)((u8 *)(s) + 4);
            m = s->mgr;
            s->stopping = 1;
            s->vp.follow = 0;
            if (s->vp.frame != m->frame) {
                alSndpSetSound(&m->player, m->handles[s->index]);
                if (alSndpGetState(&m->player) != 0) {
                    alSndpStop(&m->player);
                }
                s->sound = -1;
            }
            return (s16)i;
        }
        v++;
    }
    return -1;
}
#endif

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoicePool_IsSoundPlaying) */
s32 SndVoicePool_IsSoundPlaying(RecordRecAt4V3 *record, s32 value) {
    s32 i;
    SlotRecAtA8 *slot = record->slots;
    for (i = 0; i < 16; i++, slot++) {
        if (slot->used != -1 && record->header->local != i && slot->value == value) {
            return 1;
        }
    }
    return 0;
}
#endif

 /* size 0xCC */



#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoicePool_StopVoice) */
void SndVoicePool_StopVoice(sptr arg0, s16 arg1) {
    CAudioWorld *temp_a0;
    SndVoice *var_s0;
    void *var_s1;

    /* voice arg1 of the table at arg0 */
    var_s0 = (SndVoice *) ((arg1 * 0xCC) + arg0);
    var_s0 = (SndVoice *) &((SndVoicePool *) var_s0)->voices[0];
    temp_a0 = var_s0->mgr;
    var_s0->stopping = 1;
    var_s0->vp.follow = 0;
    var_s1 = &temp_a0->player;
    if (var_s0->vp.frame != temp_a0->frame) {
        alSndpSetSound(var_s1, temp_a0->handles[var_s0->index]);
        if (alSndpGetState(var_s1) != 0) {
            alSndpStop((sptr) var_s1);
        }
        var_s0->sound = -1;
    }
}
#endif

/* Clears every channel's state. */
void SndVoicePool_ClearFollow(SndVoicePool *arg0) {
    s32 var_v0;
    SndVoice *var_a0;

    var_a0 = arg0->voices;
    var_v0 = 0x10;
    do {
        var_a0->vp.follow = 0;
        var_v0 -= 1;
        var_a0++;
    } while (var_v0 >= 0);
}

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoice_SetPosition) */
void SndVoice_SetPosition(SndVoice *arg0, CVector3 *arg1, sptr arg2) {
    arg0->vp.pos = *arg1;
    arg0->vp.follow = arg2;
}
#endif

typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ s32 unkA8;
} Owner;

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoice_SetSoundId) */
void SndVoice_SetSoundId(Owner *arg0, s32 arg1) {
    arg0->unkA8 = arg1;
}
#endif

SndEnv *CAudioWorld__GetPanEnvelope(CAudioWorld *m, s32 id);
s32 CAudioWorld__GetStereo(CAudioWorld *m);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoice_Setup) */
void SndVoice_Setup(SndVoice *v, SoundEffectInfo *info, s32 flags) {
    f32 first;
    s16 index;
    s16 value;

    SndVoice_InitPitch(v, info, flags);
    /* the ROM passes flags too (a2) */
    ((void (*)(SndVoice *, SoundEffectInfo *, s32))SndVoice_InitVolume)(v, info, flags);
    v->vp.pan = 0x40;
    index = info->unkC;
    if (index != -1) {
        v->env3 = *CAudioWorld__GetPanEnvelope(v->mgr, index);
        first = HalfToFloat(v->env3.unk2);
        v->env3Step = (first - HalfToFloat(v->env3.target)) / v->env3.frames;
        v->env3Value = HalfToFloat(v->env3.unk2);
        v->vp.pan = (s16)(s32)HalfToFloat(v->env3.unk2);
        v->hasEnv = 1;
        return;
    }
    v->hasEnv = 0;
    if (v->soundId < 0x100) {
        if (v->reset == 0 && CAudioWorld__GetStereo(v->mgr) != 0) {
            value = (s8)v->mgr->cueVolume;
            goto store_value;
        }
    } else if (v->reset == 0 && CAudioWorld__GetStereo(v->mgr) != 0) {
        value = Sound_CalcPan(&v->vp.pos, v->mgr->listener);
store_value:
        v->vp.pan = value;
    }
}
#endif

/* 0xCC-byte slot; 16 of them follow the owner pointer. */
typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 id;          /* -1 when the slot is free */
    /* 0x0C */ u8 padC[0xCC - 0xC];
} Slot0027403C;
typedef struct {
    /* 0x000 */ u8 pad0[0x102];
    /* 0x102 */ s16 skipIndex;
} Owner0027403C;

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoicePool_Update) */
void SndVoicePool_Update(void **arg0) {
    s32 var_s0;
    s32 var_s2;
    void *var_s1;

    var_s2 = 0;
    var_s1 = ((__typeof__(arg0))((s8 *)arg0 + 4));
    var_s0 = 0;
    do {
        if ((((Slot0027403C *)var_s1)->id != -1) && (((Owner0027403C *)*arg0)->skipIndex != var_s0)) {
            SndVoice_Update((sptr) var_s1);
            var_s2 += 1;
        }
        var_s0 += 1;
        var_s1 += 0xCC;
    } while (var_s0 < 0x10);
    sSndVoicesActive = var_s2;
    if (sSndVoicesPeak < var_s2) {
        sSndVoicesPeak = var_s2;
    }
}
#endif

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoicePool_FindSoundWithOwner) */
int SndVoicePool_FindSoundWithOwner(ObjRecAtCV4 *obj, int keyA, int keyB) {
    int i;
    int *keys = &obj->slots[0].keyB;
    for (i = 0; i < 16; i++) {
        if (keys[i * 0x33 - 43] != -1 && obj->owner->local != i && keys[i * 0x33 - 3] == keyA &&
            keys[i * 0x33] == keyB) {
            return i;
        }
    }
    return -1;
}
#endif

/* Stop every voice whose key matches. */
void SndVoicePool_StopSound(SndVoicePool *set, s32 key) {
    s32 i;
    SndVoice *voice = set->voices;

    for (i = 0; i < 17; voice++, i++) {
        if (voice->soundId == key) {
            CAudioWorld *owner = voice->mgr;
            void *sound;

            voice->stopping = 1;
            voice->vp.follow = 0;
            if (voice->vp.frame != owner->frame) {
                sound = &owner->player;
                alSndpSetSound(sound, owner->handles[voice->index]);
                if (alSndpGetState(sound)) {
                    alSndpStop(sound);
                }
                voice->sound = -1;
            }
        }
    }
}

/* Stop every voice whose second key matches. */
void SndVoicePool_StopByOwner(SndVoicePool *set, s32 key) {
    s32 i;
    SndVoice *voice = set->voices;

    for (i = 0; i < 17; voice++, i++) {
        if (voice->unkB4 == key) {
            CAudioWorld *owner = voice->mgr;
            void *sound;

            voice->stopping = 1;
            voice->vp.follow = 0;
            if (voice->vp.frame != owner->frame) {
                sound = &owner->player;
                alSndpSetSound(sound, owner->handles[voice->index]);
                if (alSndpGetState(sound)) {
                    alSndpStop(sound);
                }
                voice->sound = -1;
            }
        }
    }
}

/* Stop every voice in any of the groups in mask. */
void SndVoicePool_StopByFlags(SndVoicePool *set, u32 mask) {
    s32 i;
    SndVoice *voice = set->voices;

    for (i = 0; i < 17; voice++, i++) {
        if (voice->flags & mask) {
            CAudioWorld *owner = voice->mgr;
            void *sound;

            voice->stopping = 1;
            voice->vp.follow = 0;
            if (voice->vp.frame != owner->frame) {
                sound = &owner->player;
                alSndpSetSound(sound, owner->handles[voice->index]);
                if (alSndpGetState(sound)) {
                    alSndpStop(sound);
                }
                voice->sound = -1;
            }
        }
    }
}

/* De-faked in round 21 (j4): the draft returned through identical if/else arms; returning
 * the early case first gives the ROM's layout. */

/* Distance falloff: 1 - d^2 / sSoundHearingRangeSq inside, 0 outside. */
f32 Sound_DistanceFalloff(CVector3 *a, CVector3 *b) {
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 dz = a->z - b->z;
    f32 d = dx * dx + dy * dy + dz * dz;

    if (sSoundHearingRangeSq <= d) {
        return 0.0f;
    }
    return 1.0f - d / sSoundHearingRangeSq;
}

#define TABLE sPitchRatioTable

/* Linear interpolation in a table sampled every 60 units from -6000. */
f32 Sound_CentsToPitch(f32 x) {
    f32 pos = (x + 6000.0f) * 0.016666668f;
    s16 i = pos;
    f32 fi = i;
    s16 j;

    f32 a;

    if (fi != pos) {
        j = i + 1;
        a = TABLE[i];
        return a + (pos - fi) * (TABLE[j] - a);
    }
    return TABLE[i];
}
s16 Sound_RandomVary(s16 arg0, s16 arg1);

s16 Sound_RandomVary(s16 arg0, s16 arg1) {
    f32 temp_f20;
    f32 temp_f21;
    temp_f21 = (f32)arg1;
    temp_f20 = (f32)arg0;
    return (s16)(s32)(Random_Blend(temp_f21, temp_f20) < 0.0f
                          ? temp_f20 - Random_Blend(temp_f21, temp_f20)
                          : temp_f20 + Random_Blend(temp_f21, temp_f20));
}

extern f32 Random_Float(f32 arg0);
f32 Sound_RandomVolume(u8 arg0, u8 arg1);

f32 Sound_RandomVolume(u8 arg0, u8 arg1) {
    f32 first;
    f32 second;
    first = arg0 * 0.01f;
    second = arg1 * 0.01f;
    if (first == 1.0f) {
        return 1.0f;
    }
    if (second == first) {
        return first;
    }
    return first + Random_Float(second - first);
}

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

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoice_SetVolume) */
void SndVoice_SetVolume(SndVoice *arg0, s32 arg1) {
    CAudioWorld *temp_s0;
    sptr temp_s0_2;

    temp_s0 = arg0->mgr;
    temp_s0_2 = (sptr) &temp_s0->player;
    alSndpSetSound(temp_s0_2, temp_s0->handles[arg0->index]);
    alSndpSetVol(temp_s0_2, (s32) (s16) (s32) ((f32) arg1 * arg0->volumeScale));
}
#endif
void SndVoice_SetPitch(void *arg0, f32 arg1);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoice_SetPitch) */
void SndVoice_SetPitch(void *arg0, f32 arg1) {
    RecordCode8025A3EC *slot = arg0;
    OwnerCode8025A3EC8025B5F0 *owner;
    sptr temp_s0;
    void *temp_s0_2;
    f32 scaled;
    temp_s0 = slot->owner;
    owner = (OwnerCode8025A3EC8025B5F0 *)temp_s0;
    temp_s0_2 = owner->sound;
    alSndpSetSound(temp_s0_2, owner->samples[slot->index]);
    scaled = arg1 * (slot->f34);
    scaled = scaled * (slot->fB8);
    alSndpSetPitch(temp_s0_2, scaled);
}
#endif

typedef struct {
    /* 0x00 */ s32 index;
    /* 0x04 */ u8 pad4[0xB0 - 0x4];
    /* 0xB0 */ s32 header;
} Model;

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoice_SetPan) */
void SndVoice_SetPan(Model *arg0, s32 arg1) {
    sptr var_s0;
    s32 var_s1;
    s16 *temp_v0;

    var_s1 = arg1;
    var_s0 = arg0->header;
    temp_v0 = (s16 *) (var_s0 + (arg0->index * 2));
    var_s0 += 0x84;
    alSndpSetSound(var_s0, temp_v0[0xDC / 2]);
    alSndpSetPan(var_s0, var_s1 & 0xFF);
}
#endif

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoice_GetState) */
void SndVoice_GetState(Model *arg0) {
    sptr temp_s0;
    sptr temp_v1;

    temp_v1 = arg0->header;
    temp_s0 = temp_v1 + 0x84;
    alSndpSetSound(temp_s0, ((s16 *) (temp_v1 + (arg0->index << 1)))[0xDC / 2]);
    alSndpGetState(temp_s0);
}
#endif

#include "structs/game_object.h"

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoice_Play) */
void SndVoice_Play(CGameObjectInstance *obj) {
    alSndpPlay(obj->m_rpObjectHeader + 0x84);
}
#endif

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoice_Stop) */
void SndVoice_Stop(SndVoice *arg0) {
    CAudioWorld *temp_a0;
    void *temp_s0;

    temp_a0 = arg0->mgr;
    arg0->stopping = 1;
    arg0->vp.follow = 0;
    temp_s0 = &temp_a0->player;
    if (arg0->vp.frame != temp_a0->frame) {
        alSndpSetSound(temp_s0, temp_a0->handles[arg0->index]);
        if (alSndpGetState(temp_s0) != 0) {
            alSndpStop((sptr) temp_s0);
        }
        arg0->sound = -1;
    }
}
#endif

/* Releases the voice slot and resets the voice. */
#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoice_Release) */
void SndVoice_Release(SndVoice *arg0) {
    sptr temp_a0;
    u8 *var_s1;

    temp_a0 = (sptr) arg0->mgr;
    var_s1 = (u8 *) (temp_a0 + 0x7C);       /* slots - 0x60 */
    alSndpDeallocate((void *) (temp_a0 + 0x84), *(s16 *) (var_s1 + (arg0->index << 1) + 0x60));
    var_s1 += arg0->index * 2;
    *(s16 *) (var_s1 + 0x60) = -1;          /* owner->slots[index] */
    arg0->vp.priority = 0;
    arg0->id = -1;
    arg0->unk8 = -1;
    arg0->soundId = -1;
    arg0->vp.channel = -1;
    arg0->unkB4 = -1;
}
#endif

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoice_InitPan) */
void SndVoice_InitPan(SndVoice *v, SoundEffectInfo *info) {
    f32 d;
    s16 id;
    v->vp.pan = 0x40;
    id = info->unkC;
    if (id != -1) {
        v->env3 = *CAudioWorld__GetPanEnvelope(v->mgr, id);
        d = HalfToFloat(v->env3.unk2);
        do { v->env3Step = (d - HalfToFloat(v->env3.target)) / (f32)v->env3.frames;
        v->env3Value = HalfToFloat(v->env3.unk2);
        v->vp.pan = HalfToFloat(v->env3.unk2);
        v->hasEnv = 1; return; } while (0);
    } else {
        v->hasEnv = 0;
        if (v->soundId < 0x100) {
            if (v->reset == 0 && CAudioWorld__GetStereo(v->mgr) != 0) {
                v->vp.pan = (s8)v->mgr->cueVolume;
            }
        } else if (v->reset == 0 && CAudioWorld__GetStereo(v->mgr) != 0) {
            v->vp.pan = Sound_CalcPan(&v->vp.pos, v->mgr->listener);
        }
    }
}
#endif

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndVoicePool_ReleaseVoice) */
s32 SndVoicePool_ReleaseVoice(SndVoicePool *p, s16 idx) {
    SndVoice *v;
    CAudioWorld *m;
    SndCore *c;
    (v) = (SndVoice *)((idx) * 0xCC + (uptr)(p)); (v) = (SndVoice *)((u8 *)(v) + 4);
    m = v->mgr;
    c = (SndCore *)((u8 *)m + 0x7C);
    alSndpDeallocate(&m->player, c->snd[v->index]);
    c->snd[v->index] = -1;
    v->vp.priority = 0;
    v->id = -1;
    v->unk8 = -1;
    v->soundId = -1;
    v->vp.channel = -1;
    v->unkB4 = -1;
    return 0;
}
#endif
