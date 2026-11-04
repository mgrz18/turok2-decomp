/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AUDIO/8025A3EC.CPP, func_8025A3CC_de, with Turok 2's layouts
 * (emitter layout as in src/us/code/func_00272278.c). */
#include "common.h"
#include "structs/sound.h"
#include "structs/camera.h"

extern f32 D_800B2A30; /* hearing range squared */

f32 func_0026D210(s32 x);
s32 func_00284174(void);
f32 func_00211774(f32 value, f32 target, f32 rate);
void func_00293E60(void *sndp, s16 id);
void func_00293E70(void *sndp, s16 vol);
s32 func_00293580(void *sndp);
void func_00293EC0(void *sndp);

#define MAX(a, b) ((a) < (b) ? (b) : (a))

void func_002726DC(SndVoice *o) {
    SndVoiceParams *vp;
    SndEnv *env;
    SndMgr *settings;
    SndMgr *ctx;
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
    D_800B2A30 = rangeSq;
    switch (o->vp.distanceMode) {
        case 2:
            D_800B2A30 = rangeSq + rangeSq * 0.5f;
            break;
        case 1:
            D_800B2A30 = rangeSq + rangeSq * 0.25f;
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
        if (D_800B2A30 <= d2) {
            f = 0.0f;
        } else {
            f = 1.0f - d2 / D_800B2A30;
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
        if (func_00284174() != 0) {
            vp->scale = func_00211774(vp->scale, g, 0.017f);
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
            if (down ? (o->env2Value < func_0026D210(env->target)) : (func_0026D210(env->target) < o->env2Value)) {
                o->env2Value -= o->env2Step;
                if (down) {
                    if (func_0026D210(env->target) < o->env2Value) {
                        o->env2Value = func_0026D210(env->target);
                    }
                } else if (o->env2Value < func_0026D210(env->target)) {
                    o->env2Value = func_0026D210(env->target);
                    if (o->env2Value == 0.0f) {
                        ctx = o->mgr;
                        o->stopping = 1;
                        o->vp.follow = 0;
                        if (o->vp.frame != ctx->frame) {
                            func_00293E60(&ctx->player, ctx->handles[o->index]);
                            if (func_00293580(&ctx->player) != 0) {
                                func_00293EC0(&ctx->player);
                            }
                            o->sound = -1;
                        }
                    }
                }
            }
        }
        level = func_0026D210(o->env2.level) * o->env2Value;
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
    func_00293E60(sndp, o->mgr->handles[slot]);
    func_00293E70(sndp, (s32)((f32)vi * o->volumeScale));
}
