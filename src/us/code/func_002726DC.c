/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AUDIO/8025A3EC.CPP, func_8025A3CC_de, with Turok 2's layouts
 * (emitter layout as in src/us/code/func_00272278.c). */
#include "common.h"

typedef struct {
    f32 x, y, z;
} SndVec3;

typedef struct {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ SndVec3 pos;
    /* 0x120 */ u8 pad120[0x510 - 0x120];
    /* 0x510 */ f32 hearRange;
} SndListener;

typedef struct {
    /* 0x0000 */ u8 pad0[0x84];
    /* 0x0084 */ u8 player[0x58]; /* ALSndPlayer */
    /* 0x00DC */ s16 snd[16];
    /* 0x00FC */ u8 padFC[8];
    /* 0x0104 */ s32 frame;
    /* 0x0108 */ u8 pad108[0x2B98 - 0x108];
    /* 0x2B98 */ SndListener *listener;
    /* 0x2B9C */ u8 pad2B9C[4];
    /* 0x2BA0 */ f32 volA;
    /* 0x2BA4 */ f32 volB;
    /* 0x2BA8 */ f32 volC;
    /* 0x2BAC */ u8 pad2BAC[0xC];
    /* 0x2BB8 */ s32 halve;
} SndCtx;

typedef struct {
    /* 0x00 */ s32 w0;
    /* 0x04 */ s32 w4;
    /* 0x08 */ s32 vol;
    /* 0x0C */ u8 padC[0x1C - 0xC];
    /* 0x1C */ f32 scale;
    /* 0x20 */ f32 f20;
    /* 0x24 */ u8 pad24[0x34 - 0x24];
    /* 0x34 */ SndVec3 pos;
    /* 0x40 */ s32 f40; /* tracked */
    /* 0x44 */ s8 mode;
    /* 0x48 */ f32 f48;
    /* 0x4C */ f32 f4C;
} SndVoice;

typedef struct {
    /* 0x0 */ u16 w0;
    /* 0x2 */ u16 w1;
    /* 0x4 */ u16 w2;
    /* 0x6 */ s16 w3;
    /* 0x8 */ s16 delay;
    /* 0xA */ u16 w5;
} SndEnv;

typedef struct {
    /* 0x00 */ s32 slot;
    /* 0x04 */ s32 handle;
    /* 0x08 */ s32 w8;
    /* 0x0C */ s32 id;
    /* 0x10 */ SndVoice vp;
    /* 0x60 */ u8 pad60[0x74 - 0x60];
    /* 0x74 */ SndEnv env2;
    /* 0x80 */ f32 env2A;
    /* 0x84 */ f32 env2Slope;
    /* 0x88 */ u8 pad88[0xA4 - 0x88];
    /* 0xA4 */ u32 flags;
    /* 0xA8 */ s32 kind;
    /* 0xAC */ s32 done;
    /* 0xB0 */ SndCtx *ctx;
    /* 0xB4 */ u8 padB4[0xC0 - 0xB4];
    /* 0xC0 */ s32 resetFlag;
    /* 0xC4 */ s32 unkC4;
    /* 0xC8 */ f32 unkC8;
} SndEmitter;

extern f32 D_800B2A30; /* hearing range squared */

f32 func_0026D210(s32 x);
s32 func_00284174(void);
f32 func_00211774(f32 value, f32 target, f32 rate);
void func_00293E60(void *sndp, s16 id);
void func_00293E70(void *sndp, s16 vol);
s32 func_00293580(void *sndp);
void func_00293EC0(void *sndp);

#define MAX(a, b) ((a) < (b) ? (b) : (a))

void func_002726DC(SndEmitter *o) {
    SndVoice *vp;
    SndEnv *env;
    SndCtx *settings;
    SndCtx *ctx;
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

    hear = o->ctx->listener->hearRange;
    range = MAX(hear, 3072.0f);
    rangeSq = range * MAX(hear, 3072.0f);
    D_800B2A30 = rangeSq;
    switch (o->vp.mode) {
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

    settings = o->ctx;
    if (o->flags & 0x200) {
        master = settings->volC;
    } else {
        master = settings->volA;
        if (o->unkC4 != 0 && settings->halve != 0) {
            master *= 0.5f;
        }
    }

    if ((o->flags & 0x400) && o->resetFlag == 0 && !(o->flags & 0x800)) {
        {
            SndVec3 *p = &vp->pos;
            SndVec3 *q = &o->ctx->listener->pos;
            d2 = (p->x - q->x) * (p->x - q->x) + (p->y - q->y) * (p->y - q->y) + (p->z - q->z) * (p->z - q->z);
        }
        if (D_800B2A30 <= d2) {
            f = 0.0f;
        } else {
            f = 1.0f - d2 / D_800B2A30;
        }
        switch (vp->mode) {
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
            if (o->env2Slope < 0.0f) {
                down = 1;
            }
            if (down ? (o->env2A < func_0026D210(env->w2)) : (func_0026D210(env->w2) < o->env2A)) {
                o->env2A -= o->env2Slope;
                if (down) {
                    if (func_0026D210(env->w2) < o->env2A) {
                        o->env2A = func_0026D210(env->w2);
                    }
                } else if (o->env2A < func_0026D210(env->w2)) {
                    o->env2A = func_0026D210(env->w2);
                    if (o->env2A == 0.0f) {
                        ctx = o->ctx;
                        o->done = 1;
                        o->vp.f40 = 0;
                        if (o->vp.w0 != ctx->frame) {
                            func_00293E60(ctx->player, ctx->snd[o->slot]);
                            if (func_00293580(ctx->player) != 0) {
                                func_00293EC0(ctx->player);
                            }
                            o->handle = -1;
                        }
                    }
                }
            }
        }
        level = func_0026D210(o->env2.w0) * o->env2A;
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

    sndp = o->ctx->player;
    slot = o->slot;
    /* An empty statement macro (a compiled-out debug check, probably): the
     * scheduling block boundary it makes is what puts the multiply after the
     * loads. */
    do {
    } while (0);
    vi = vol * master;
    func_00293E60(sndp, o->ctx->snd[slot]);
    func_00293E70(sndp, (s32)((f32)vi * o->unkC8));
}
