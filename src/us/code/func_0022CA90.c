/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_3.CPP, func_8027D4A8_de, with Turok 2's layouts. */

#include "common.h"

typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

/* Two halfwords of an effect's parameters, or the pair as one word. */
typedef struct ActionParams {
    /* 0x00 */ s16 upper;
    /* 0x02 */ s16 lower;
    /* 0x04 */ s32 extra;
} ActionParams;

typedef struct ParticleEvents {
    /* 0x00 */ ActionParams params[14];
    /* 0x70 */ u16 models[14];
    /* 0x8C */ u16 effects[14];
    /* 0xA8 */ u16 sounds[14];
} ParticleEvents;

typedef struct ParticleDef {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x14];
    /* 0x18 */ ParticleEvents *events;
} ParticleDef;

typedef struct Target Target;

typedef struct Particle {
    /* 0x000 */ u8 pad0[2];
    /* 0x002 */ u16 type;
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[8];
    /* 0x018 */ Vec3f velocity;
    /* 0x024 */ u8 pad24[0x40 - 0x24];
    /* 0x040 */ u32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ ParticleDef *def;
    /* 0x0FC */ u8 padFC[0x118 - 0xFC];
    /* 0x118 */ s32 handle;
    /* 0x11C */ u8 pad11C[0x150 - 0x11C];
    /* 0x150 */ Vec3f direction;
    /* 0x15C */ u8 pad15C[0x195 - 0x15C];
    /* 0x195 */ s8 hitSurface;
    /* 0x196 */ u8 pad196[0x1AC - 0x196];
    /* 0x1AC */ s8 space;
    /* 0x1AD */ s8 shake;
    /* 0x1AE */ u8 pad1AE[0x1B5 - 0x1AE];
    /* 0x1B5 */ u8 drawWhenHidden;
} Particle;

/* The point of the last collision test (D_800F1CE0 + 8). */
typedef struct CollisionPoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ u8 padC[0xDC - 0xC];
    /* 0xDC */ Vec3f normal;
} CollisionPoint;

typedef struct EffectInfo {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[4];
} EffectInfo;

extern Target *D_800F1CE0;              /* the target hit */
extern CollisionPoint D_800F1CE8;
extern Vec3f D_800F1DB8;                /* the direction of the hit */
extern u8 D_8011AAF1;
extern EffectInfo D_800AD084[];
extern u8 D_800F8928[];
extern u8 D_801102B8[];
extern u8 D_8010FF34[];

s32 func_00245724(Target *target);
s32 func_002325F0(Particle *p);
void func_00236EA0(void *source, Particle *p, s32 effect, s32 arg3, Vec3f pos, ActionParams params);
void func_00229340(Particle *p, s32 model, s32 arg2, s32 enabled);
void func_00275544(s16 sound, Vec3f pos, s32 arg2, s32 arg3);
void func_002364A4(void *list, Vec3f pos, Vec3f normal, s32 kind, s32 arg4);
void func_0020F040(Vec3f *v);
f32 func_002119FC(f32 lo, f32 hi);
void func_0020EF60(Vec3f *out, Vec3f *in, f32 scale);
void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);
void func_002327F4(Particle *p);
void func_00234E48(void *list, Particle *p);
void func_00228B4C(void *list, s32 handle);
void func_0022F1C8(void *list, Particle *p);
void func_00232B40(Particle *p);

/* Puts the particle at the hit point, flying along the hit direction, and pulls it back
 * along that direction by a random distance. */
static __inline__ void placeAtHit(Particle *p) {
    Vec3f dir;

    p->pos = D_800F1CE8.pos;
    p->velocity = D_800F1DB8;
    dir = D_800F1DB8;
    func_0020F040(&dir);
    p->direction = dir;
    func_0020EF60(&dir, &dir, func_002119FC(-49.92f, 0.0f));
    func_0020EEF8(&p->pos, &p->pos, &dir);
}

/* A particle hit the world (no target): leaves a decal, sticks or bounces off the surface
 * by type, plays the impact's effect, model and sound, and dies when its descriptor says so.
 * A particle under player control plays its own impact instead and always dies. */
void func_0022CA90(Particle *p) {
    ActionParams params;
    Vec3f normal;
    ActionParams controlledParams;
    Target *source;
    ParticleDef *definition;
    s32 kind;

    source = D_800F1CE0;
    definition = p->def;
    kind = func_00245724(source);
    if (kind == 5) {
        kind = 4;
        p->flags |= 2;
    }
    if (D_8011AAF1 != 0 && func_002325F0(p)) {
        ParticleDef *controlledDefinition = p->def;
        s32 controlledKind, model, sound, effect;

        if (func_002325F0(p)) {
            controlledKind = 12;
        } else {
            controlledKind = 10;
        }
        model = controlledDefinition->events->models[controlledKind];
        effect = controlledDefinition->events->effects[controlledKind];
        controlledParams = controlledDefinition->events->params[controlledKind];
        sound = controlledDefinition->events->sounds[controlledKind];
        if (effect != 0xFFFF) {
            func_00236EA0(p, p, effect, -1, p->pos, controlledParams);
        }
        if (model != 0xFFFF) {
            func_00229340(p, model, p->space, 1);
        }
        if (sound != 0xFFFF) {
            func_00275544(sound, p->pos, 0, -1);
        }
        goto remove;
    }
    switch (kind) {
        case 0:
        case 2:
        case 3:
            if (definition->flags & 0x800000) {
                func_002364A4(D_801102B8, D_800F1CE8.pos, D_800F1CE8.normal, kind, 0);
            }
            break;
    }
    if (p->type == 0x56 || p->type == 0x126 || p->type == 0xCD || p->type == 0xF) {
        p->flags |= 0x20000;
        placeAtHit(p);
        p->shake = 0x7F;
        if (p->type == 0x56 && kind == 2) {
            p->flags |= 0x4000000;
        }
        if (p->drawWhenHidden) {
            func_002327F4(p);
            func_00234E48(D_801102B8, p);
        }
        if (p->handle) {
            func_00228B4C(D_8010FF34, p->handle);
            p->handle = 0;
        }
    }
    normal = D_800F1CE8.normal;
    func_0020F040(&normal);
    func_0020EF60(&normal, &normal, 1.024f);
    func_0020EEF8(&p->pos, &p->pos, &normal);
    {
        s32 model, sound, effect;

        model = definition->events->models[kind];
        effect = definition->events->effects[kind];
        params = definition->events->params[kind];
        sound = definition->events->sounds[kind];
        if (effect != 0xFFFF) {
            void *eventSource = source;

            if (D_800AD084[effect].flags & 2) {
                eventSource = p;
            }
            func_00236EA0(eventSource, p, effect, -1, D_800F1CE8.pos, params);
        }
        if (model != 0xFFFF) {
            func_00229340(p, model, -8, 1);
        }
        if (sound != 0xFFFF) {
            func_00275544(sound, D_800F1CE8.pos, 0, -1);
        }
    }
    if (!(p->flags & 0x30000) && p->hitSurface == 1) {
remove:
        func_0022F1C8(D_800F8928, p);
        func_00232B40(p);
    }
}
