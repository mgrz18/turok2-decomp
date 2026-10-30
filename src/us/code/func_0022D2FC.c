/* One source file from func_0022D2FC to func_0022F350: the particle code.
 * func_0022E094's .rodata starts at 0x800A631C and func_0022F350's at
 * 0x800A63A4, both 4 past an 8-byte boundary, and their jump tables are
 * 8-aligned only counted from 0x800A6250, where func_0022D2FC's pool opens
 * (func_0022DCBC's, at 0x800A62F4, continues it too). Alone, each object put
 * its table 4 bytes off.
 *
 * func_0022D2FC does not match yet (nonmatching/engine/func_0022D2FC.c), so
 * it comes from its asm, and its literals and jump table from
 * src/us/asm/rodata/func_0022D2FC.s, ahead of the pools the C below emits.
 * The four C parts were written apart: each keeps its own view of the shared
 * globals and structs. func_0022E094 and func_0022F350 are after Rage Wars
 * (Particle_DrawSprite and func_802800C0_de). */
#include "common.h"
#include "include_asm.h"
#include "structs/game_object.h"

INCLUDE_RODATA("src/us/asm/rodata", func_0022D2FC);

INCLUDE_ASM("us/asm/nonmatchings/code/func_0022D2FC", func_0022D2FC);

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
/* MIN as a statement expression: its value lives in its own temporary. */
#define MINF(a, b) ({ f32 _m = (a); if (!(_m <= (b))) _m = (b); _m; })

typedef union {
    struct { u32 w0, w1; } words;
} Gfx;

typedef struct { f32 x, y, z; } Vec3;
typedef struct { f32 x, y, z, w; } Quat;
typedef struct { f32 m[4][4]; } Mtx44;

/* The leading 0x34 bytes of a game object, as the collision test reads them. */
typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3 pos;
    /* 0x10 */ void *region;
    /* 0x14 */ u8 pad14[0x4];
    /* 0x18 */ Vec3 velocity;
    /* 0x24 */ u8 pad24[0x10];
} ObjectHead; /* size 0x34 */

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ f32 shadowScale;
} ShadowParams;

typedef struct {
    /* 0x00 */ u32 flags;
    u8 pad4[0x10];
    /* 0x14 */ s32 bigShadow;
    u8 pad18[0xC];
    /* 0x24 */ ShadowParams *params;
} ShadowDef;

typedef struct {
    u8 pad0[0x4];
    /* 0x004 */ Vec3 pos;
    /* 0x010 */ void *region;
    u8 pad14[0xE4];
    /* 0x0F8 */ ShadowDef *def;
    u8 padFC[0x20];
    /* 0x11C */ f32 fade;
    u8 pad120[0x8];
    /* 0x128 */ s16 fadeTime;
    u8 pad12A[0xA];
    /* 0x134 */ f32 radius;
} ShadowActor;

/* The result of the last collision test (D_800F1CE0). */
typedef struct {
    /* 0x00 */ struct Actor *hit;
    u8 pad4[0x4];
    /* 0x08 */ Vec3 pos;
    /* 0x14 */ s32 frame;
    u8 pad18[0x7E - 0x18];
    /* 0x7E */ u8 team;
    u8 pad7F[0xD8 - 0x7F];
    /* 0xD8 */ Vec3 direction;
    /* 0xE4 */ Vec3 normal;
} CollisionResult;

struct CollisionInfo;
extern u8 D_800F7148[];
extern struct CollisionInfo D_800F1B40;
extern CollisionResult D_800F1CE0;
extern f32 D_800DD78C;
extern Gfx *D_800D8E24;

void *func_00206950(void *pool, s32 count);
void func_0026A9C0(Quat *out, void *region);
f32 func_00267648(ShadowActor *actor, s32 arg1);    /* ground height */
s32 func_00239F00(void *obj, Vec3 to, struct CollisionInfo *info);
void func_0020E810(Quat *out, Vec3 *dir);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void func_00210F68(Quat *out, Quat *a, Quat *b);
void func_0020F510(Vec3 *v, f32 arg1);
void func_00211114(Quat *rot, Mtx44 *m);
void func_0021034C(Mtx44 *m, f32 x, f32 y, f32 z);  /* scale */
void func_00210318(Mtx44 *m, f32 x, f32 y, f32 z);  /* translate */
void func_0020D274(Mtx44 *m, void *dst);
void func_002086BC(void *mtx, s32 arg1, u32 alpha);

/* Draws the blob shadow under an actor: on the ground of its region, or
 * where a ray cast straight down from it lands, tilted to the surface. */
void func_0022DCBC(ShadowActor *actor) {
    Mtx44 mtx;
    Vec3 pos;
    Quat rot;
    Quat tilt;
    Quat spin;
    ObjectHead probe;
    void *gmtx;
    f32 ground;
    f32 height;
    f32 alpha;
    f32 a;
    f32 size;

    gmtx = func_00206950(D_800F7148, 1);
    if (gmtx == NULL) {
        return;
    }
    if (actor->region != NULL) {
        func_0026A9C0(&rot, actor->region);
        ground = func_00267648(actor, 0);
    } else {
        probe = *(ObjectHead *) actor;
        probe.pos.y += 10.24f;
        pos.x = probe.pos.x;
        pos.y = probe.pos.y - 102400.0f;
        pos.z = probe.pos.z;
        probe.velocity.x = 0;
        probe.velocity.y = 0;
        probe.velocity.z = 0;
        func_00239F00(&probe, pos, &D_800F1B40);
        if (D_800F1CE0.hit == NULL) {
            return;
        }
        func_0020E810(&tilt, &D_800F1CE0.normal);
        spin.x = D_800DD78C = func_002982F0(0.785398245f);
        spin.y = 0.0f;
        spin.z = 0.0f;
        spin.w = func_002974C0(0.785398245f);
        func_00210F68(&rot, &spin, &tilt);
        ground = D_800F1CE0.pos.y;
    }
    height = actor->pos.y - ground;
    a = 200.0f - height * 0.30517578f;
    alpha = MAX(MIN(a, 255.0f), 0.0f);
    size = actor->radius * 7.68f;
    if (actor->def->bigShadow) {
        size *= 8.0f;
    }
    size *= actor->def->params->shadowScale;
    size = MAX(MINF(size - height * 0.1f, size), 0.0f);
    if ((actor->def->flags & 1) && actor->fadeTime > 0.0f) {
        alpha *= (actor->fadeTime - actor->fade) / actor->fadeTime;
    }
    pos.x = actor->pos.x;
    pos.z = actor->pos.z;
    pos.y = ground;
    func_0020F510(&pos, 20000.0f);
    func_00211114(&rot, &mtx);
    func_0021034C(&mtx, size, 1.0f, size);
    func_00210318(&mtx, pos.x, pos.y, pos.z);
    func_0020D274(&mtx, gmtx);
    { Gfx *_g = (Gfx *) (D_800D8E24++); _g->words.w0 = 0xE7000000; _g->words.w1 = 0; }
    { Gfx *_g = (Gfx *) (D_800D8E24++); _g->words.w0 = 0xE3000A01; _g->words.w1 = 0x100000; }
    func_002086BC(gmtx, 1, (u8) alpha);
    { Gfx *_g = (Gfx *) (D_800D8E24++); _g->words.w0 = 0xE7000000; _g->words.w1 = 0; }
    { Gfx *_g = (Gfx *) (D_800D8E24++); _g->words.w0 = 0xE3000A01; _g->words.w1 = 0; }
}

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_6.CPP, Particle_DrawSprite, with Turok 2's layouts. */

/* Draws one sprite particle for a view: fades particles hidden behind
 * geometry in or out, blends its colours towards the descriptor's targets,
 * picks the animation frame, builds its matrix (once per frame for a shared
 * camera, per view otherwise), selects the texture and render mode, works out
 * its alpha from life, fade-in, fade-out and distance from the view, and
 * emits the colour and draw commands. */


#define GFX_CMD(a, b) { Gfx *_g = D_800D8E24++; _g->words.w0 = (a); _g->words.w1 = (b); }

typedef struct { u8 pad0[0x40]; } Mtx;

typedef struct {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ Gfx *commands;
} GfxTask;

/* The leading 0x34 bytes of a game object, as the collision test reads them. */
typedef struct {
    /* 0x00 */ u16 pad0;
    /* 0x02 */ u16 type;
    /* 0x04 */ Vec3 pos;
    /* 0x10 */ u8 pad10[0x24];
} ParticleHead; /* size 0x34 */

typedef struct {
    /* 0x0 */ u8 kind;
    /* 0x1 */ u8 pad1;
    /* 0x2 */ u8 shiftS;
    /* 0x3 */ u8 shiftT;
} AnimFrame;

typedef struct {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ AnimFrame *frame;
} AnimInfo;

typedef struct {
    /* 0x0 */ u8 pad0[0x6];
    /* 0x6 */ u8 prim[3];
    /* 0x9 */ u8 env[3];
} ParticleColors;

typedef struct {
    /* 0x0 */ u8 pad0[0x5];
    /* 0x5 */ s8 rangeMode;
    /* 0x6 */ u8 pad6[0x3];
    /* 0x9 */ s8 fadeIn;
    /* 0xA */ s8 fadeOut;
    /* 0xB */ u8 padB[0x3];
    /* 0xE */ s8 unkE;
} ParticleFade;

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x4];
    /* 0x08 */ s8 animMode;
    /* 0x09 */ u8 pad9[0x34 - 0x9];
    /* 0x34 */ ParticleColors *colors;
    /* 0x38 */ ParticleFade *fade;
} ParticleDesc;

typedef struct {
    /* 0x000 */ ParticleHead inst;
    /* 0x034 */ u8 pad34[0x40 - 0x34];
    /* 0x040 */ u32 flags;
    /* 0x044 */ u8 pad44[0x48 - 0x44];
    /* 0x048 */ Mtx mtx[2];
    /* 0x0C8 */ u8 padC8[0xF8 - 0xC8];
    /* 0x0F8 */ ParticleDesc *desc;
    /* 0x0FC */ s32 model;
    /* 0x100 */ u8 pad100[0x11C - 0x100];
    /* 0x11C */ f32 time;
    /* 0x120 */ f32 frame;
    /* 0x124 */ u8 pad124[0x128 - 0x124];
    /* 0x128 */ s16 life;
    /* 0x12A */ s8 builtFrame;
    /* 0x12B */ s8 randomFrame;
    /* 0x12C */ u8 pad12C[0x174 - 0x12C];
    /* 0x174 */ f32 alpha;
    /* 0x178 */ u8 pad178[0x1AE - 0x178];
    /* 0x1AE */ u8 prim[3];
    /* 0x1B1 */ u8 env[3];
    /* 0x1B4 */ u8 opacity;
} Particle;

typedef struct {
    /* 0x000 */ u8 pad0[0x8];
    /* 0x008 */ s32 index;
    /* 0x00C */ u8 padC[0x24 - 0xC];
    /* 0x024 */ s32 unk24;
    /* 0x028 */ u8 pad28[0x114 - 0x28];
    /* 0x114 */ Vec3 eye;
    /* 0x120 */ u8 pad120[0x20C - 0x120];
    /* 0x20C */ f32 viewMtx[4][4];
    /* 0x24C */ u8 pad24C[0x510 - 0x24C];
    /* 0x510 */ f32 fadeRange;
} ParticleView;

struct CollisionInfo;

extern Gfx *D_800D8E24;             /* display list head */
extern u32 D_800C2214;              /* command buffer length, in commands */
extern GfxTask *D_800F7070;
extern f32 D_800B6D28;              /* frame time */
extern s32 D_800B6D1C;              /* matrix buffer index */
extern s32 D_801198E0;
extern void *D_801198D0; /* the current view */
extern u8 D_8011AAEF;
extern s32 D_8011AAD8;
extern struct CollisionInfo D_800F1B40;
extern u8 D_800F1930[];             /* the sprite's four vertices */
extern u8 D_800F7148[];             /* matrix pool */

extern void *func_0020798C(s32 *ref);
extern void func_002079D8(void *model, AnimInfo *info);
extern void func_00206B30(s32 *ref, void *model, s32 frame, s32 a3, s32 shiftS, s32 shiftT, s32 a6, s32 a7, s32 a8);
extern ParticleHead *func_00281194(ParticleView *view);
extern s32 func_00239F00(void *obj, Vec3 to, struct CollisionInfo *info);
extern void func_0022D2FC(void *particle, void *mtx, void *camera, f32 depth);
extern void func_0020F85C(void *m, void *in, void *out);
extern void *func_00206950(void *pool, s32 count);
extern void func_00207A70(s32 mode);
extern void func_00207FEC(s32 mode);
extern void func_002051F4(s32 arg0, void *model);
extern s32 func_002113A4(void);

static __inline__ s32 Particle_Occluded(Particle *particle, ParticleView *view) {
    ParticleHead *inst;
    s32 hit;

    inst = func_00281194(view);
    if (inst != 0 && view->unk24 == 0) {
        ParticleHead saved;
        saved = *inst;
        inst->pos = view->eye;
        hit = func_00239F00(inst, particle->inst.pos, &D_800F1B40);
        *inst = saved;
    } else {
        ParticleHead probe;
        probe = particle->inst;
        probe.pos = view->eye;
        hit = func_00239F00(&probe, particle->inst.pos, &D_800F1B40);
    }
    return hit;
}

static __inline__ s32 Particle_ColorMode(Particle *particle) {
    s32 mode = 0;

    if (particle->desc->flags & 0x2000000) {
        if (particle->flags & 2) {
            mode = 2;
        } else if (D_8011AAEF == 2) {
            mode = 1;
        }
    }
    return mode;
}

static __inline__ s32 Particle_Scale(f32 value, f32 div) {
    return (value < 0.0f) ? 0 : (s32)((s32)value / div);
}

#define CLAMP_U8(v) (u32)(((v) > 255.0f ? 255.0f : (v)) < 0.0f ? 0.0f : ((v) > 255.0f ? 255.0f : (v)))
#define LERP(to, from, t) (((to) - (from)) * (t) + (from))

void func_0022E094(Particle *particle, ParticleView *view) {
    AnimInfo info;
    u8 env[3];
    u8 prim[3];
    Vec3 viewPos;
    void *model;
    ParticleDesc *desc;
    void *mtx;
    f32 fade;
    f32 ratio;
    f32 dist;
    s32 hit;
    s32 alpha;
    s8 duration;
    s32 mode;
    s32 frame;
    s32 time;
    s32 textured;
    s32 shiftS;
    s32 shiftT;
    u8 *envTo;
    u8 *primTo;
    u8 *envFrom;
    u8 *primFrom;

    if (D_800C2214 - ((u32)D_800D8E24 - (u32)D_800F7070->commands) / sizeof(Gfx) < 3000) {
        return;
    }
    if (particle->time < 0.0f) {
        return;
    }
    model = func_0020798C(&particle->model);
    if (model == 0) {
        return;
    }
    desc = particle->desc;
    if ((desc->flags & 0x80000) && desc->animMode == 1) {
        if (particle->inst.type != 0x60) {
            particle->time = 0.0f;
        }
        hit = Particle_Occluded(particle, view);
        if (!hit) {
            fade = particle->opacity + D_800B6D28 * 128.0f;
            alpha = (fade > 255.0f) ? 255 : (s32)fade;
        } else {
            fade = particle->opacity - D_800B6D28 * 128.0f;
            alpha = (fade < 0.0f) ? 0 : (s32)fade;
        }
        particle->opacity = alpha;
        if (alpha == 0.0f) {
            func_002051F4(0, model);
            return;
        }
    }

    particle->flags |= 8 << view->index;
    particle->alpha = 255.0f;
    ratio = particle->time / particle->life > 1.0f ? 1.0f : particle->time / particle->life;
    primFrom = particle->prim;
    envFrom = particle->env;
    primTo = desc->colors->prim;
    envTo = desc->colors->env;
    env[0] = CLAMP_U8(LERP(envTo[0], envFrom[0], ratio));
    env[1] = CLAMP_U8(LERP(envTo[1], envFrom[1], ratio));
    env[2] = CLAMP_U8(LERP(envTo[2], envFrom[2], ratio));
    prim[0] = CLAMP_U8(LERP(primTo[0], primFrom[0], ratio));
    prim[1] = CLAMP_U8(LERP(primTo[1], primFrom[1], ratio));
    prim[2] = CLAMP_U8(LERP(primTo[2], primFrom[2], ratio));

    mode = Particle_ColorMode(particle);
    func_002079D8(model, &info);
    frame = particle->frame;
    time = particle->time;
    switch (desc->animMode) {
        case 0:
            if (frame >= info.count - 1) {
                time = particle->life;
                frame = info.count - 1;
                particle->time = time;
            }
            break;
        case 1:
            if (frame >= info.count) {
                frame = info.count - 1;
            }
            break;
        case 2:
            frame %= info.count;
            break;
        case 3:
            if (info.count * 2 - 2 > 0) {
                frame %= info.count * 2 - 2;
            } else {
                frame = 0;
            }
            if (frame >= info.count) {
                frame = (info.count * 2 - 2) - frame;
            }
            break;
        case 4:
            frame = func_002113A4() % info.count;
            break;
        case 5:
            if (particle->randomFrame == -1) {
                particle->randomFrame = func_002113A4() % info.count;
            }
            frame = particle->randomFrame;
            break;
    }

    if (!(particle->flags & 0x100000)) {
        if (D_801198E0 == 1) {
            ParticleView *camera = D_801198D0;
            Vec3 local;
            f32 depth;
            func_0020F85C(camera->viewMtx, &particle->inst.pos, &local);
            depth = local.z;
            if (depth < 0.0f) {
                depth = -depth;
            }
            func_0022D2FC(particle, &particle->mtx[D_800B6D1C], camera, depth);
        } else {
            func_0022D2FC(particle, &particle->mtx[D_800B6D1C], 0, 0.0f);
        }
        particle->builtFrame = frame;
        particle->flags |= 0x100000;
    }
    func_0020F85C(view->viewMtx, &particle->inst.pos, &viewPos);
    dist = viewPos.z;
    if (dist < 0.0f) {
        dist = -dist;
    }
    if (D_801198E0 == 1) {
        GFX_CMD(0xDA380003, (u32)&particle->mtx[D_800B6D1C])
    } else {
        mtx = func_00206950(D_800F7148, 1);
        if (mtx == 0) {
            func_002051F4(0, model);
            return;
        }
        func_0022D2FC(particle, mtx, view, dist);
        GFX_CMD(0xDA380003, (u32)mtx)
    }

    textured = particle->flags & 0x800000;
    shiftS = 1 << (((desc->flags & 0x100) ? 7 : 6) + info.frame->shiftS);
    shiftT = 1 << (((desc->flags & 0x200) ? 7 : 6) + info.frame->shiftT);
    func_00206B30(&particle->model, model, frame, 0, shiftS, shiftT, 1, 1, 0);
    switch (info.frame->kind) {
        case 0:
        case 1:
            func_00207A70(0x18);
            break;
        case 3:
            func_00207A70(0x1D);
            break;
    }
    if (textured) {
        func_00207FEC(0xF);
        GFX_CMD(0xD7FF0002, (shiftS << 16) | (shiftT & 0xFFFF))
    } else if ((desc->flags & 0x400) && desc->fade->unkE != 4) {
        func_00207FEC(0xF);
    } else if (desc->flags & 0x4000) {
        func_00207FEC(!(D_8011AAD8 & 0x200) ? 0xD : 0x11);
    } else {
        func_00207FEC((D_8011AAD8 & 0x200) ? 0x12 : 0x13);
    }

    alpha = 0xFF;
    if ((desc->flags & 1) && particle->life > 0) {
        alpha = (particle->life - time) * 255.0f / particle->life;
    }
    duration = desc->fade->fadeIn;
    if (time < duration) {
        alpha = alpha * (time + 1.0f) / (duration + 1.0f);
    }
    {
        f32 left = particle->life - time;
        if ((left < 0.0f ? 0.0f : left) < desc->fade->fadeOut) {
            alpha = Particle_Scale(alpha * (particle->life - time), desc->fade->fadeOut + 1.0f);
        }
    }
    {
        f32 range = view->fadeRange;
        f32 half;
        f32 delta;
        switch (desc->fade->rangeMode) {
            case 2:
                alpha = alpha * (((range - dist) * 10.0f / range) > 1.0f ? 1.0f : ((range - dist) * 10.0f / range));
                break;
            case 0:
                alpha = alpha * (((range - dist) * 5.0f / range) > 1.0f ? 1.0f : ((range - dist) * 5.0f / range));
                break;
            case 1:
                half = range * 0.5f;
                delta = half - dist;
                alpha = alpha * (delta < 0.0f ? half + delta : half - delta) / half;
                break;
        }
    }

    if (info.frame->kind == 3) {
        switch (mode) {
            case 0:
            default:
                GFX_CMD(0xFB000000, (env[0] << 24) | (env[1] << 16) | (env[2] << 8) | 0xFF)
                break;
            case 2:
                GFX_CMD(0xFB000000, (env[0] << 24) | (env[0] << 16) | (env[2] << 8) | 0xFF)
                break;
            case 1:
                GFX_CMD(0xFB000000, (env[1] << 24) | (env[0] << 16) | (env[2] << 8) | 0xFF)
                break;
        }
    }
    alpha = (alpha > 255 ? 255 : alpha) < 0 ? 0 : (alpha > 255 ? 255 : alpha);
    GFX_CMD(0x01004008, (u32)D_800F1930)
    if (textured) {
        GFX_CMD(0xD7FF0002, (shiftS << 16) | (shiftT & 0xFFFF))
    }
    particle->alpha = (alpha * particle->opacity) >> 8;
    switch (mode) {
        case 0:
        default:
            GFX_CMD(0xFA000000, (prim[0] << 24) | (prim[1] << 16) | (prim[2] << 8) | ((u32)particle->alpha & 0xFF))
            break;
        case 2:
            GFX_CMD(0xFA000000, (prim[0] << 24) | (prim[0] << 16) | (prim[2] << 8) | ((u32)particle->alpha & 0xFF))
            break;
        case 1:
            GFX_CMD(0xFA000000, (prim[1] << 24) | (prim[0] << 16) | (prim[2] << 8) | ((u32)particle->alpha & 0xFF))
            break;
    }
    GFX_CMD(0x06000204, 0x00040600)
    func_002051F4(0, model);
}


/* Engine actor instance (only the fields used here). */
typedef struct EffectActor {
    /* 0x000 */ u8 kind;
    u8 pad1;
    /* 0x002 */ u16 typeId;
    u8 pad4[0x3C];
    /* 0x040 */ u32 flags;
    u8 pad44[0xC4];
    /* 0x108 */ struct EffectActor *child;
    /* 0x10C */ u8 *parent;
    /* 0x110 */ s32 *liveCount;
    u8 pad114[0x4];
    /* 0x118 */ s32 sound;
    u8 pad11C[0x99];
    /* 0x1B5 */ u8 registered;
    u8 pad1B6[0xA];
    /* 0x1C0 */ void *list;
} EffectActor;

extern u8 D_800F8928[];
extern u8 D_801102B8[];
extern u8 D_8010FF34[];
s16 *func_0025398C(u8 *, s32);
void func_00232B40(EffectActor *);
void func_002327F4(EffectActor *);
void func_00234E48(void *, EffectActor *);
void func_00228B4C(void *, s32);
void CList__Remove(void *list, void *node);
void CList__AddHead(void *list, void *node);

void func_0022F1C8(u8 *mgr, EffectActor *actor) {
    EffectActor *child;
    u8 *owner;
    u32 flags;
    u8 kind;

    flags = actor->flags;
    if (!(flags & 0x100)) {
        return;
    }
    if (!(flags & 0x800)) {
        if (actor->typeId == 0x68) {
            owner = *(u8 **) (actor->parent + 0x1A8);
            *(s16 *) (owner + *func_0025398C(owner, 0x15) * 2 + 0x934) = 1;
        }
        child = actor->child;
        if (child != NULL && (kind = child->kind) == 2 && (child->flags & 0x100) &&
            (child->typeId == kind || child->typeId == 0x111)) {
            func_0022F1C8(D_800F8928, child);
            func_00232B40(child);
        }
        func_00232B40(actor);
        if (actor->registered) {
            func_002327F4(actor);
            func_00234E48(D_801102B8, actor);
        }
        if (actor->sound != 0) {
            func_00228B4C(D_8010FF34, actor->sound);
            actor->sound = 0;
        }
        if (actor->liveCount != NULL) {
            (*actor->liveCount)--;
        }
        actor->flags &= ~0x02000100;
        CList__Remove(actor->list, actor);
        actor->list = NULL;
        CList__AddHead(mgr + 0xEC00, actor);
        if (actor->flags & 0x01000000) {
            CList__Remove(mgr + 0xEC14, actor);
        }
    }
}

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_8.CPP, func_802800C0_de, with Turok 2's layouts. */

/* Spawns the particles that template block 2 of the particle system lists for
 * a kind: for each template in the kind's range it filters by player mask,
 * existing owner particles, distance to the current player, mode, team flags
 * and a percent chance, builds a spawn matrix (source matrix, mirror,
 * translation or rotation), then for a random count allocates particles, fills
 * their timing, spread, colour, render and direction fields from the
 * template's packed floats, links them into their group, aims shots at the
 * last collision target for kinds 0x2D and 0x4F, and plays their sound.
 * Returns the number spawned. */

typedef Vec3 Vec3f;

/* Two half-floats: base and random range (see func_0026D128). */
typedef struct {
    /* 0x0 */ u16 base;
    /* 0x2 */ u16 range;
} PackedRange;

typedef struct {
    /* 0x00 */ s16 lifeBase;
    /* 0x02 */ s16 lifeRange;
    /* 0x04 */ s8 chance;
    /* 0x05 */ s8 group;
    /* 0x06 */ s8 countBase;
    /* 0x07 */ s8 countRange;
    /* 0x08 */ u8 listIndex;
    /* 0x09 */ u8 pad9[2];
    /* 0x0B */ s8 unkB;
    /* 0x0C */ u8 padC;
    /* 0x0D */ s8 spread;
    /* 0x0E */ s8 mode;
    /* 0x0F */ u8 event;
} EntryParams;

typedef struct {
    /* 0x00 */ u8 rgb0[3];
    /* 0x03 */ u8 rgb1[3];
    /* 0x06 */ u8 pad6[6];
    /* 0x0C */ s8 hueRange;
    /* 0x0D */ s8 satRange;
    /* 0x0E */ s8 valRange;
} EntryColor;

typedef struct {
    /* 0x00 */ PackedRange size;
    /* 0x04 */ PackedRange speed;
    /* 0x08 */ u16 unk8[4];
} EntryMotion;

typedef struct {
    /* 0x00 */ u8 pad0[0x94];
    /* 0x94 */ u16 type;
} EntryModel;

typedef struct Entry {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s8 unk6;
    /* 0x07 */ s8 unk7;
    /* 0x08 */ s8 unk8;
    /* 0x09 */ u8 unk9;
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 unkB;
    /* 0x0C */ s16 sound;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ u8 playerMask;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ EntryModel *model;
    /* 0x1C */ PackedRange *offset;
    /* 0x20 */ PackedRange *unk20;
    /* 0x24 */ PackedRange *unk24;
    /* 0x28 */ PackedRange *direction;
    /* 0x2C */ PackedRange *unk2C;
    /* 0x30 */ EntryMotion *motion;
    /* 0x34 */ EntryColor *color;
    /* 0x38 */ EntryParams *params;
} Entry; /* 0x3C bytes */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 count;
    /* 0x08 */ Entry entries[1];
} EntryBlock;

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 pad7;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
} Render;

typedef struct {
    /* 0x00 */ s32 type;
} ActorDef;

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x10];
    /* 0x14 */ s32 large;
} SourceModel;

typedef struct Actor {
    /* 0x000 */ u8 type;
    /* 0x001 */ u8 pad1[3];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0x40 - 0x18];
    /* 0x040 */ u32 flags;
    /* 0x044 */ u8 pad44[4];
    /* 0x048 */ Mtx44 matrices[1];
    /* 0x088 */ u8 pad88[0x98 - 0x88];
    /* 0x098 */ Mtx44 *bones;
    /* 0x09C */ u8 pad9C[0xF8 - 0x9C];
    /* 0x0F8 */ SourceModel *model;
    /* 0x0FC */ u8 padFC[0x130 - 0xFC];
    /* 0x130 */ f32 unk130;
    /* 0x134 */ f32 unk134;
    /* 0x138 */ u8 pad138[0x140 - 0x138];
    /* 0x140 */ s32 ai;
} Actor;

typedef struct Owner {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad1[0x7F];
    /* 0x80 */ void *ownerId;
} Owner;

typedef struct Effect {
    /* 0x000 */ u8 pad0[2];
    /* 0x002 */ u16 kind;
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ void *region;
    /* 0x014 */ u8 pad14[4];
    /* 0x018 */ Vec3f velocity;
    /* 0x024 */ s32 unk24;
    /* 0x028 */ s32 unk28;
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ s32 unk30;
    /* 0x034 */ Vec3f attachPos;
    /* 0x040 */ u32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ Entry *entry;
    /* 0x0FC */ u32 bits;
    /* 0x100 */ u8 pad100[4];
    /* 0x104 */ void *ownerId;
    /* 0x108 */ Actor *source;
    /* 0x10C */ Actor *owner;
    /* 0x110 */ s32 *refCount;
    /* 0x114 */ s32 target;
    /* 0x118 */ s32 sound;
    /* 0x11C */ f32 spread;
    /* 0x120 */ f32 age;
    /* 0x124 */ f32 unk124;
    /* 0x128 */ s16 life;
    /* 0x12A */ u8 unk12A;
    /* 0x12B */ s8 unk12B;
    /* 0x12C */ f32 unk12C[6];
    /* 0x144 */ Vec3f startPos;
    /* 0x150 */ Vec3f direction;
    /* 0x15C */ f32 unk15C[6];
    /* 0x174 */ f32 alpha;
    /* 0x178 */ f32 rot[6];
    /* 0x190 */ Render render;
    /* 0x1AC */ s8 mode;
    /* 0x1AD */ u8 shake;
    /* 0x1AE */ u8 rgb0[3];
    /* 0x1B1 */ u8 rgb1[3];
    /* 0x1B4 */ u8 alphaByte;
    /* 0x1B5 */ u8 unk1B5;
    /* 0x1B6 */ u8 pad1B6[0x1BC - 0x1B6];
    /* 0x1BC */ f32 unk1BC;
    /* 0x1C0 */ void *list;
    /* 0x1C4 */ u8 pad1C4[4];
    /* 0x1C8 */ struct Effect *next;
    /* 0x1CC */ s32 unk1CC;
    /* 0x1D0 */ struct Effect *groupNext;
} Effect;

typedef struct {
    /* 0x00 */ Effect *head;
    /* 0x04 */ Effect *tail;
    /* 0x08 */ u8 pad8[0xC];
} EffectList;

typedef struct {
    /* 0x0000 */ u8 pad0[0xEC00];
    /* 0xEC00 */ EffectList free;
    /* 0xEC14 */ EffectList groups;
    /* 0xEC28 */ EffectList lists[3];
    /* 0xEC64 */ s32 resource;
} EffectSystem;

typedef struct {
    /* 0x000 */ u8 pad0[0x20C];
    /* 0x20C */ Mtx44 view;
} Camera;

extern s32 D_8011ACA4;              /* frozen; the current player list is 0x1414 bytes before */
extern u8 D_8011AAF1;
extern u8 D_8011AAEF;
extern CollisionResult *D_800F1ADC;
extern CollisionResult D_800F1CE0;
extern Render D_800F1BA0;
extern s32 D_801198E0;
extern s32 D_800B6D1C;
extern u8 D_800F7078[];
extern u8 D_801102B8[];
extern u8 D_8010FF34[];
extern f32 D_800ACE10[2];
extern u32 D_800AD084[];

extern void *func_002017D4(s32 resource, s32 index);
extern s32 func_00266D8C(Entry *entries, s32 count, s32 kind, s32 *first, s32 *last);
extern void func_00223A94(void *, Vec3f *);
extern void func_00225C08(void *, s32, Vec3f *);
extern s32 func_002113A4(void);
extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);
extern void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);
extern void func_0020EF60(Vec3f *out, Vec3f *in, f32 scale);
extern void func_0020F040(Vec3f *v);
extern void func_0020FAFC(Mtx44 *m, Vec3f *in, Vec3f *out);
extern void func_0020D908(Mtx44 *out, Mtx44 *in);
extern void func_0020C208(Mtx44 *out, Mtx44 *in);
extern void func_0021034C(Mtx44 *m, f32 x, f32 y, f32 z);
extern void func_00210318(Mtx44 *m, f32 x, f32 y, f32 z);
extern void func_0020FC74(Mtx44 *m, f32 x, f32 y, f32 z);
extern void func_0020FC24(Mtx44 *m, f32 x, f32 y, f32 z);
extern void func_0020F79C(Mtx44 *m);
extern void func_002673CC(Quat *q, Owner *owner, Vec3f pos);
extern void func_00211114(Quat *q, Mtx44 *m);
extern void CList__AddTail(EffectList *list, Effect *effect);
extern void func_002005D0(EffectList *list, Effect *after, Effect *effect);
extern void func_00207914(u32 *bits, s32 value);
extern f32 func_0026D128(PackedRange r);
extern f32 func_0026D210(u16 h);
extern void func_002688C8(Effect *effect, Vec3f pos, Vec3f *outPos, void **outRegion, s32 a, s32 b);
extern void func_00228C40(s8, u8, u8, u8, u8 *, u8 *, u8 *);
extern void func_00228FF8(u8, u8, u8, u8, u8 *, u8 *, u8 *);
extern void func_002291D4(u8, u8, u8, u8, u8 *, u8 *, u8 *);
extern void func_0025E52C(Actor *actor, s32 *ai, Vec3f *from, s32 arg3);
extern void func_0022D09C(Effect *effect, s32, s32);
extern void func_002350EC(void *, Effect *, s32);
extern s32 func_00228AB0(void *, s32);
extern void func_00232AC4(Effect *effect, s32 sound);
extern void func_00275544(s16 sound, Vec3f pos, s32 a, s32 b);

static __inline__ s32 randomCount(s32 range) {
    return range != 0 ? func_002113A4() % (range + 1) : 0;
}

static __inline__ Effect *findOwned(EffectList *list, void *ownerId, Entry *entry) {
    Effect *effect;

    effect = list->head;
    if (entry != 0) {
        for (; effect != 0; effect = effect->next) {
            if (effect->ownerId == ownerId && effect->entry == entry) {
                return effect;
            }
        }
    } else {
        for (; effect != 0; effect = effect->next) {
            if (effect->ownerId == ownerId) {
                return effect;
            }
        }
    }
    return 0;
}

static __inline__ Effect *allocateEffect(EffectSystem *sys, u8 index) {
    Effect *effect;
    EffectList *list;
    s32 i;

    if (sys->free.head == 0) {
        for (i = 0; i <= index; i++) {
            if ((&sys->lists[i])->tail != 0) {
                func_0022F1C8(sys, (&sys->lists[i])->tail);
                break;
            }
        }
    }
    effect = sys->free.head;
    if (effect == 0) {
        return 0;
    }
    CList__Remove(&sys->free, effect);
    list = &sys->lists[index];
    CList__AddHead(list, effect);
    effect->list = list;
    effect->groupNext = 0;
    effect->unk1CC = 0;
    effect->flags = 0x100;
    return effect;
}

static __inline__ void aimEffect(Effect *effect, Vec3f *vel) {
    Mtx44 targetMtx;
    Mtx44 targetInv;
    Vec3f up;
    Actor *targetActor;
    s32 targetTeam;
    CollisionResult *scene;

    if (D_800F1CE0.frame != -1) {
        effect->mode = -4;
        targetActor = D_800F1CE0.hit;
        targetTeam = D_800F1CE0.team;
        func_0020D908(&targetMtx, &targetActor->bones[D_800F1CE0.frame]);
        func_0020C208(&targetInv, &targetMtx);
        scene = &D_800F1CE0;
        func_0020F85C(&targetInv, &scene->pos, &effect->attachPos);
        up = D_800F1CE0.direction;
        func_0020F040(&up);
        func_0020FAFC(&targetInv, &up, vel);
        effect->target = (s32) targetActor;
        effect->flags |= 0x10000;
        effect->shake = D_800F1CE0.frame;
        if (targetActor->def->type == 1) {
            if (effect->owner != 0) {
                func_0025E52C(targetActor, &targetActor->ai, &effect->owner->pos, 3);
            } else {
                func_0025E52C(targetActor, &targetActor->ai, &scene->pos, 3);
            }
        }
        if (effect->kind == 0x56 && targetTeam == 2) {
            effect->flags |= 0x4000000;
        }
    }
}

s32 func_0022F350(EffectSystem *sys, Actor *source, Owner *owner, s32 *refCount, s32 arg4, s32 kind,
                  Vec3f arg6, Quat rotation, Vec3f pos, void *region, s32 arg10, s32 arg11) {
    Mtx44 mtx;
    Vec3f dir;
    Vec3f dirOut;
    Vec3f offset;
    Vec3f offsetOut;
    Vec3f delta;
    Quat quat;
    CollisionResult target;
    Vec3f aimDelta;
    s32 first;
    s32 last;
    u8 h, sv, v;
    u8 r, g, b;
    u8 r0, g0, b0;
    u8 r1, g1, b1;
    s32 count;
    s32 j;
    s32 n;
    s32 mode;
    s32 i;
    Actor *anchor;
    void *ownerId;
    s32 extra;
    s32 life;
    s32 layer;
    s32 ok;
    s32 hue;
    u8 sat;
    u8 val;
    s32 spread;
    u32 *bits;
    u8 mask;
    s32 resource;
    EntryBlock *block;
    s32 numEntries;
    Entry *templates;
    Entry *entries;
    Entry *entry;
    Effect *effect;
    Render *render;
    Vec3f *vel;
    Camera *camera;
    f32 distSq;
    f32 scale;
    f32 depth;
    s32 sound;
    s32 *global = &D_8011ACA4;

    if (global[0] != 0) {
        return 0;
    }
    count = 0;
    resource = sys->resource;
    D_800F1ADC = &target;
    block = func_002017D4(resource, 1);
    numEntries = block->count;
    templates = block->entries;
    if (kind == 0xFC) {
        func_00223A94(D_800F7078, &pos);
    }
    if (func_00266D8C(templates, numEntries, kind, &first, &last) != 0) {
        entries = ((EntryBlock *) func_002017D4(resource, 2))->entries;
        anchor = 0;
        /* The current player list, D_80119890, addressed from D_8011ACA4. */
        if (((Actor **) global)[-0x505] != 0) {
            anchor = ((Actor **) global)[-0x505];
        }
        ownerId = 0;
        if (owner != 0) {
            switch (owner->type) {
                case 1:
                case 2:
                    ownerId = owner;
                    break;
                case 0:
                    ownerId = owner->ownerId;
                    owner = 0;
                    break;
            }
        }
        mask = 1;
        if (D_8011AAF1 != 0) {
            mask = 2;
        }
        for (i = first; i <= last; i++) {
            entry = &entries[i];
            ok = (entry->playerMask & mask) != 0;
            if ((entry->flags & 0x80) && ownerId != 0) {
                if (findOwned(&sys->lists[(s8) entry->params->listIndex], ownerId, entry) != 0) {
                    ok = 0;
                }
            }
            if (anchor != 0) {
                func_0020EF2C(&delta, &pos, &anchor->pos);
            } else {
                delta.x = delta.y = delta.z = 0.0f;
            }
            distSq = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
            if (entry->flags & 0x400000) {
                if (262144.0f <= distSq) {
                    ok = 0;
                }
            } else if (entry->flags & 0x200000) {
                if (distSq <= 262144.0f) {
                    ok = 0;
                }
            }
            switch (entry->params->mode) {
                case 6:
                    mode = -7;
                    break;
                case 5:
                    mode = -6;
                    break;
                case 0:
                    mode = -4;
                    break;
                case 1:
                    mode = -5;
                    break;
                default:
                    switch (arg10) {
                        case -5:
                        case -3:
                        case -2:
                            mode = arg10;
                            break;
                        default:
                            mode = -3;
                            break;
                    }
                    break;
                case 3:
                    if (0.0f <= (f32) arg10) {
                        mode = arg10;
                    } else {
                        ok = 0;
                        mode = 0;
                    }
                    break;
                case 4:
                    mode = arg10;
                    break;
            }
            if (entry->flags & 0x2000000) {
                if (D_8011AAEF == 0 || !(arg11 & 1)) {
                    ok = 0;
                }
            }
            if (entry->flags & 0x4000000) {
                if (D_8011AAEF != 0) {
                    ok = 0;
                }
            }
            if (entry->params->chance != 100) {
                if (func_002113A4() % 1000 + 1 > entry->params->chance * 10) {
                    ok = 0;
                }
            }
            if (!ok) {
                continue;
            }
            if (entry->params->event != 0) {
                func_00225C08(D_800F7078, entry->params->event, &pos);
            }
            extra = 0;
            if (entry->flags & 0x8000) {
                switch (source->type) {
                    case 2:
                        if (!(source->flags & 0x100000)) {
                            if (D_801198E0 == 1) {
                                camera = D_801198D0;
                                func_0020F85C(&camera->view, &source->pos, &aimDelta);
                                depth = aimDelta.z;
                                if (depth < 0.0f) {
                                    depth = -depth;
                                }
                                func_0022D2FC(source, &source->matrices[D_800B6D1C], camera, depth);
                            } else {
                                func_0022D2FC(source, &source->matrices[D_800B6D1C], 0, 0.0f);
                            }
                            source->flags |= 0x100000;
                        }
                        func_0020D908(&mtx, &source->matrices[D_800B6D1C]);
                        scale = 0.09765625f;
                        if (source->model->large != 0) {
                            scale = 2.857143f;
                        }
                        func_0021034C(&mtx, scale, scale, scale);
                        if (source->model->flags & 8) {
                            func_00210318(&mtx, 0.0f, -source->unk130 * source->unk134, 0.0f);
                        }
                        break;
                    case 1:
                        extra = 0x400000;
                        func_0020FC74(&mtx, -1.0f, 1.0f, -1.0f);
                        break;
                    default:
                        func_0020FC24(&mtx, source->pos.x, source->pos.y, source->pos.z);
                        break;
                }
            } else if (entry->flags & 0x1000000) {
                func_0020F79C(&mtx);
            } else {
                if (entry->flags & 0x40000) {
                    func_002673CC(&quat, owner, pos);
                } else {
                    quat = rotation;
                }
                func_00211114(&quat, &mtx);
            }
            n = entry->params->countBase + randomCount(entry->params->countRange);
            for (j = 0; j < n; j++) {
                life = entry->params->lifeBase + randomCount(entry->params->lifeRange);
                if (life == 0) {
                    continue;
                }
                effect = allocateEffect(sys, entry->params->listIndex);
                if (effect == 0) {
                    break;
                }
                count++;
                func_00207914(&effect->bits, entry->unk4);
                effect->velocity.x = 0;
                effect->velocity.y = 0;
                effect->velocity.z = 0;
                effect->unk24 = 0;
                effect->unk28 = 0;
                effect->unk2C = 0;
                effect->unk30 = 0;
                effect->kind = kind;
                effect->entry = entry;
                effect->mode = mode;
                if (source->type == 2) {
                    effect->source = source;
                } else {
                    effect->source = 0;
                }
                effect->ownerId = ownerId;
                effect->owner = (Actor *) owner;
                effect->refCount = refCount;
                if (refCount != 0) {
                    (*refCount)++;
                }
                effect->target = arg4;
                effect->flags = (effect->flags & ~0xE600) | (arg11 | extra);
                bits = &effect->bits;
                layer = 0;
                if (effect->entry->flags & 0x2000000) {
                    if (effect->flags & 2) {
                        layer = 2;
                    } else if (D_8011AAEF == 2) {
                        layer = 1;
                    }
                }
                *bits |= layer << 30;
                effect->unk1B5 = 0;
                spread = entry->params->spread;
                if (spread < 0) {
                    effect->spread = spread;
                } else {
                    effect->spread = -(f32) randomCount(spread);
                }
                effect->rot[0] = func_0026D128(entry->unk2C[2]) * 6.2831855f;
                effect->rot[2] = func_0026D128(entry->unk2C[1]) * 0.41887906f;
                effect->rot[4] = func_0026D128(entry->unk2C[0]);
                effect->rot[1] = func_0026D128(entry->unk2C[5]) * 6.2831855f;
                effect->rot[3] = func_0026D128(entry->unk2C[4]) * 0.41887906f;
                effect->rot[5] = func_0026D128(entry->unk2C[3]);
                if (entry->flags & 2) {
                    effect->age = randomCount(life);
                } else {
                    effect->age = 0.0f;
                }
                effect->unk124 = entry->params->unkB;
                effect->life = life;
                effect->unk12A = 0;
                effect->unk12B = -1;
                dir.x = func_0026D128(entry->direction[0]);
                dir.y = func_0026D128(entry->direction[1]);
                dir.z = func_0026D128(entry->direction[2]);
                func_0020F040(&dir);
                if (dir.x != 0.0f || dir.y != 0.0f || dir.z != 0.0f) {
                    func_0020F85C(&mtx, &dir, &dirOut);
                } else {
                    dirOut = dir;
                }
                if (mode == -8) {
                    effect->direction = D_800F1CE0.normal;
                    func_0020F040(&effect->direction);
                } else {
                    effect->direction = dirOut;
                }
                effect->unk12C[0] = func_0026D128(entry->unk24[0]);
                effect->unk12C[1] = func_0026D128(entry->unk24[2]);
                effect->unk12C[2] = func_0026D128(entry->unk24[4]);
                effect->unk12C[3] = func_0026D128(entry->unk24[1]);
                effect->unk12C[4] = func_0026D128(entry->unk24[3]);
                effect->unk12C[5] = func_0026D128(entry->unk24[5]);
                /* Both branches are the same (as in Rage Wars); GCC merges them after loop
                 * optimisation, whose decisions still see both copies. */
                if (entry->unk14 != 0) {
                    effect->unk15C[0] = func_0026D128(entry->unk20[0]);
                    effect->unk15C[1] = func_0026D128(entry->unk20[2]);
                    effect->unk15C[2] = func_0026D128(entry->unk20[4]);
                    effect->unk15C[3] = func_0026D128(entry->unk20[1]);
                    effect->unk15C[4] = func_0026D128(entry->unk20[3]);
                    effect->unk15C[5] = func_0026D128(entry->unk20[5]);
                } else {
                    effect->unk15C[0] = func_0026D128(entry->unk20[0]);
                    effect->unk15C[1] = func_0026D128(entry->unk20[2]);
                    effect->unk15C[2] = func_0026D128(entry->unk20[4]);
                    effect->unk15C[3] = func_0026D128(entry->unk20[1]);
                    effect->unk15C[4] = func_0026D128(entry->unk20[3]);
                    effect->unk15C[5] = func_0026D128(entry->unk20[5]);
                }
                if ((entry->flags & 0x80000) && entry->unk8 == 1) {
                    effect->alphaByte = 0;
                    effect->alpha = 0.0f;
                } else {
                    effect->alphaByte = 0xFF;
                    effect->alpha = 255.0f;
                }
                effect->region = region;
                effect->pos = pos;
                offset.x = func_0026D128(entry->offset[0]);
                offset.y = func_0026D128(entry->offset[1]);
                offset.z = func_0026D128(entry->offset[2]);
                if (offset.x != 0.0f || offset.y != 0.0f || offset.z != 0.0f) {
                    func_0020F85C(&mtx, &offset, &offsetOut);
                    if (!(entry->flags & 0x8000)) {
                        func_0020EEF8(&offsetOut, &pos, &offsetOut);
                    }
                    if (effect->region != 0) {
                        func_002688C8(effect, offsetOut, &effect->pos, &effect->region, (s8) entry->unkB,
                                      (s8) entry->unk9);
                    } else {
                        effect->pos = offsetOut;
                    }
                }
                effect->startPos = effect->pos;
                effect->attachPos = effect->pos;
                func_0020EF60(&effect->velocity, &dirOut, func_0026D128(entry->motion->speed));
                if (entry->flags & 0x20) {
                    func_0020EEF8(&effect->velocity, &effect->velocity, &arg6);
                }
                hue = 0;
                if (entry->color->hueRange != 0) {
                    hue = func_002113A4() % (entry->color->hueRange * 2) - (u8) entry->color->hueRange;
                }
                sat = entry->color->satRange != 0 ? func_002113A4() % entry->color->satRange : 0;
                val = entry->color->valRange != 0 ? func_002113A4() % entry->color->valRange : 0;
                r = entry->color->rgb0[0];
                g = entry->color->rgb0[1];
                b = entry->color->rgb0[2];
                func_00228C40(hue, r, g, b, &h, &sv, &v);
                func_00228FF8(sat, h, sv, v, &r, &g, &b);
                func_002291D4(val, r, g, b, &r0, &g0, &b0);
                r = entry->color->rgb1[0];
                g = entry->color->rgb1[1];
                b = entry->color->rgb1[2];
                func_00228C40(hue, r, g, b, &h, &sv, &v);
                func_00228FF8(sat, h, sv, v, &r, &g, &b);
                func_002291D4(val, r, g, b, &r1, &g1, &b1);
                effect->rgb0[0] = r0;
                effect->rgb0[1] = g0;
                effect->rgb0[2] = b0;
                effect->rgb1[0] = r1;
                effect->rgb1[1] = g1;
                effect->rgb1[2] = b1;
                render = &effect->render;
                *render = D_800F1BA0;
                render->unk5 = entry->unkA;
                render->unk4 = entry->unk9;
                render->unk6 = entry->unkB;
                render->unk8 = func_0026D128(entry->motion->size);
                render->unkC = func_0026D210(entry->motion->unk8[0]);
                render->unk10 = func_0026D210(entry->motion->unk8[1]);
                render->unk14 = func_0026D210(entry->motion->unk8[2]);
                render->unk18 = func_0026D210(entry->motion->unk8[3]);
                if (kind == 0x101 || kind == 0x128) {
                    render->flags |= 4;
                }
                if (entry->flags & 0x800) {
                    render->flags |= 0x800000;
                }
                if (entry->flags & 0x40000000) {
                    render->flags |= 0x1000000;
                }
                if (entry->params->group != 3) {
                    Effect *other;

                    for (other = sys->groups.head; other != 0; other = other->groupNext) {
                        if (other->entry == effect->entry) {
                            break;
                        }
                    }
                    if (other != 0) {
                        func_002005D0(&sys->groups, other, effect);
                    } else if (effect->entry->flags & 0x2000) {
                        CList__AddHead(&sys->groups, effect);
                    } else {
                        CList__AddTail(&sys->groups, effect);
                    }
                    effect->flags |= 0x1000000;
                }
                if ((D_800AD084[entry->model->type * 2] & 1) || kind == 0x68) {
                    render->flags |= 0x80000000;
                }
                vel = &effect->velocity;
                func_0020EF60(vel, vel, D_800ACE10[0]);
                if ((kind == 0x2D || kind == 0x4F) && source->type == 2) {
                    if (D_800F1CE0.hit != 0) {
                        aimEffect(effect, vel);
                    }
                }
                effect->unk1BC = D_800ACE10[1];
                func_0022D09C(effect, 0, 0);
                if (entry->unk6 != -1) {
                    func_002350EC(D_801102B8, effect, entry->unk6);
                }
                if (entry->unk7 != -1) {
                    effect->sound = func_00228AB0(D_8010FF34, entry->unk7);
                } else {
                    effect->sound = 0;
                }
                if (!(effect->flags & 0x200000) && (sound = entry->sound) != 0) {
                    switch (effect->kind) {
                        case 0x22:
                        case 0x5F:
                        case 0x60:
                            func_00232AC4(effect, sound);
                            break;
                        default:
                            func_00275544(sound, effect->pos, 0, -1);
                            break;
                    }
                }
            }
        }
    }
    D_800F1ADC = &D_800F1CE0;
    return count;
}
