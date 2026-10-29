/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_3.CPP, Particle_HitTarget, with Turok 2's layouts. */

#include "common.h"

typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

typedef struct Bone {
    /* 0x00 */ u8 pad0[0x40];
} Bone;

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

typedef struct ActorDef {
    /* 0x00 */ s32 type;
} ActorDef;

typedef struct Target Target;
struct Target {
    /* 0x0000 */ u8 kind;
    /* 0x0001 */ u8 pad1[3];
    /* 0x0004 */ Vec3f pos;
    /* 0x0010 */ s32 unk10;
    /* 0x0014 */ ActorDef *def;
    /* 0x0018 */ u8 pad18[0x98 - 0x18];
    /* 0x0098 */ Bone *bones;
    /* 0x009C */ u8 pad9C[0xD4 - 0x9C];
    /* 0x00D4 */ u32 flags;
    /* 0x00D8 */ u8 padD8[0x140 - 0xD8];
    /* 0x0140 */ s32 aiFlags;
    /* 0x0144 */ u8 pad144[0x240 - 0x144];
    /* 0x0240 */ void (*callback)(Target *target, s32 *ai, void *particle);
    /* 0x0244 */ u8 pad244[0x12B0 - 0x244];
    /* 0x12B0 */ s32 extraUses;
};

typedef struct Particle {
    /* 0x000 */ u8 pad0[2];
    /* 0x002 */ u16 type;
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ u8 pad14[4];
    /* 0x018 */ Vec3f velocity;
    /* 0x024 */ u8 pad24[0x34 - 0x24];
    /* 0x034 */ Vec3f attachPos;
    /* 0x040 */ u32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ ParticleDef *def;
    /* 0x0FC */ u8 padFC[0x10C - 0xFC];
    /* 0x10C */ Target *owner;
    /* 0x110 */ u8 pad110[4];
    /* 0x114 */ Target *target;
    /* 0x118 */ s32 handle;
    /* 0x11C */ u8 pad11C[0x194 - 0x11C];
    /* 0x194 */ s8 mode;
    /* 0x195 */ s8 hitSurface;
    /* 0x196 */ u8 pad196[0x1AD - 0x196];
    /* 0x1AD */ s8 shake;
    /* 0x1AE */ u8 pad1AE[0x1B5 - 0x1AE];
    /* 0x1B5 */ u8 drawWhenHidden;
    /* 0x1B6 */ u8 pad1B6[0x1BC - 0x1B6];
    /* 0x1BC */ f32 scale;
} Particle;

/* The result of the last collision test. */
typedef struct CollisionResult {
    /* 0x00 */ Target *current;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ Vec3f origin;
    /* 0x14 */ s32 frame;
    /* 0x18 */ u8 pad18[0x7E - 0x18];
    /* 0x7E */ u8 mode;
    /* 0x7F */ u8 pad7F[0xD8 - 0x7F];
    /* 0xD8 */ Vec3f direction;
} CollisionResult;

typedef struct EffectInfo {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[4];
} EffectInfo;

extern CollisionResult D_800F1CE0;
extern u8 D_800F1D5D;
extern u8 D_8011AAF1;
extern EffectInfo D_800AD084[];
extern u8 D_800F8928[];
extern u8 D_801102B8[];
extern u8 D_8010FF34[];

s32 func_002682CC(Target *target);
s32 func_00245724(Target *target);
void func_0020D908(f32 *out, Bone *bone);
void func_0021B39C(Target *target, s32 *ai, f32 *matrix, Vec3f origin, Vec3f direction, s32 frame);
void func_0020C208(f32 *out, f32 *in);
void func_0020F85C(f32 *matrix, Vec3f *in, Vec3f *out);
void func_0020F040(Vec3f *v);
void func_0020FAFC(f32 *matrix, Vec3f *in, Vec3f *out);
void func_0025E52C(Target *target, s32 *ai, Vec3f *from, s32 arg3);
void func_002327F4(Particle *p);
void func_00234E48(void *list, Particle *p);
void func_00228B4C(void *list, s32 handle);
s32 func_002325F0(Particle *p);
s32 func_00268148(Target *target);
void func_00236EA0(void *source, Particle *p, s32 effect, s32 arg3, Vec3f pos, ActionParams params);
void func_00229340(Particle *p, s32 model, s32 arg2, s32 enabled);
void func_00275544(s16 sound, Vec3f pos, s32 arg2, s32 arg3);
void func_0022F1C8(void *list, Particle *p);
void func_00232B40(Particle *p);

/* Sticks the particle to the bone it hit: its position and velocity in the bone's frame. */
static __inline__ void attachParticle(Particle *p, f32 *transform, s32 action) {
    f32 localTransform[16];
    f32 inverse[16];
    Vec3f direction;
    Target *attached;
    CollisionResult *scene;

    attached = D_800F1CE0.current;
    if (transform) {
        func_0020C208(inverse, transform);
    } else {
        func_0020D908(localTransform, &attached->bones[D_800F1CE0.frame]);
        func_0020C208(inverse, localTransform);
    }
    scene = &D_800F1CE0;
    func_0020F85C(inverse, &scene->origin, &p->attachPos);
    direction = scene->direction;
    func_0020F040(&direction);
    func_0020FAFC(inverse, &direction, &p->velocity);
    p->flags |= 0x10000;
    p->target = attached;
    p->shake = D_800F1CE0.frame;
    if (attached->def->type == 1) {
        if (p->owner != NULL) {
            func_0025E52C(attached, &attached->aiFlags, &p->owner->pos, 3);
        } else {
            func_0025E52C(attached, &attached->aiFlags, &D_800F1CE0.origin, 3);
        }
    }
    if (p->type == 0x56 && action == 2) {
        p->flags |= 0x4000000;
    }
}

/* A particle hit the current collision target: attaches to it when its type sticks, then
 * plays the impact's model, effects and sound for the hit's action and removes the particle
 * when its descriptor says so. */
void func_0022C488(Particle *p) {
    ActionParams primary;
    ActionParams secondary;
    f32 boneMatrix[16];
    Target *source;
    Target *owner;
    ParticleDef *def;
    ParticleEvents *actions;
    ParticleEvents *extraActions;
    void *other;
    s32 mode;
    s32 enabled;
    s32 action;
    s32 special;
    s32 draw;
    s32 model;
    s32 effect;
    s32 sound;
    s32 extraEffect;
    u16 type;

    source = D_800F1CE0.current;
    enabled = 1;
    special = 0;
    def = p->def;
    if (func_002682CC(source)) {
        mode = p->hitSurface;
    } else {
        mode = p->mode;
    }
    if (p->unk10 == 0) {
        p->unk10 = source->unk10;
    }
    if (D_800F1CE0.frame != -1) {
        action = D_800F1CE0.mode;
        if (p->type == 13) {
            action = 4;
        }
        owner = D_800F1CE0.current;
        if (owner->flags & 0x3000000) {
            func_0020D908(boneMatrix, &owner->bones[D_800F1CE0.frame]);
            if (owner->flags & 0x1000000) {
                func_0021B39C(owner, &owner->aiFlags, boneMatrix, D_800F1CE0.origin, D_800F1CE0.direction,
                              D_800F1CE0.frame);
            }
            if (owner->flags & 0x2000000) {
                type = p->type;
                if (type == 0x12A || type == 0x132) {
                    if (owner->def->type == 11) {
                        special = owner->extraUses < 1;
                        if (special) {
                            owner->extraUses++;
                        }
                    }
                }
                if (p->type == 0x56 || p->type == 2 || p->type == 0x126 || p->type == 15 || special
                    || (p->type == 11 && (D_800F1D5D & 3) == 1)) {
                    attachParticle(p, boneMatrix, action);
                    if (p->drawWhenHidden) {
                        func_002327F4(p);
                        func_00234E48(D_801102B8, p);
                    }
                    if (p->handle) {
                        func_00228B4C(D_8010FF34, p->handle);
                        p->handle = 0;
                    }
                }
            }
        }
    } else {
        action = func_00245724(source);
    }
    if (action == 5) {
        action = 4;
        p->flags |= 2;
    }
    if (func_002325F0(p) && action < 6 && action >= 4) {
        action = 6;
    }
    actions = def->events;
    model = actions->models[action];
    effect = actions->effects[action];
    primary = actions->params[action];
    sound = def->events->sounds[action];
    if (source->kind == 1) {
        if (source->callback) {
            source->callback(source, &source->aiFlags, p);
        }
        if (func_00268148(source)) {
            enabled = 0;
        }
        if (source->aiFlags & 2) {
            enabled = 0;
        }
    }
    if (effect != 0xFFFF) {
        if (D_800AD084[effect].flags & 4) {
            if (p->type == 0x56) {
                primary.lower = primary.lower * p->scale;
            }
        }
        other = source;
        if (D_800AD084[effect].flags & 2) {
            other = p;
        }
        func_00236EA0(other, p, effect, -1, D_800F1CE0.origin, primary);
    }
    if (action != 4) {
        draw = 1;
        if (D_8011AAF1) {
            draw = source->def->type != 11;
            if (mode == p->hitSurface) {
                draw = 0;
            }
        }
        if (draw) {
            extraActions = def->events;
            extraEffect = extraActions->effects[4];
            if (extraEffect != 0xFFFF) {
                secondary = extraActions->params[4];
                if (D_800AD084[extraEffect].flags & 4) {
                    other = source;
                    if (D_800AD084[extraEffect].flags & 2) {
                        other = p;
                    }
                    func_00236EA0(other, p, extraEffect, -1, D_800F1CE0.origin, secondary);
                }
            }
        }
    }
    if (model != 0xFFFF) {
        func_00229340(p, model, -8, enabled);
    }
    if (sound != 0xFFFF) {
        func_00275544((s16) sound, D_800F1CE0.origin, 0, -1);
    }
    if (!(p->flags & 0x30000)) {
        if (mode == 1 || (effect != 0xFFFF && (D_800AD084[effect].flags & 1))) {
            func_0022F1C8(D_800F8928, p);
            func_00232B40(p);
        }
    }
}
