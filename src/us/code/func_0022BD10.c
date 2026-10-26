/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_3.CPP, func_8027C4F0_de, with Turok 2's layouts. */
#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;
typedef struct { f32 x, y, z, w; } Quatf;
typedef struct { s32 a, b; } IntPair;

typedef struct {
    /* 0x00 */ IntPair parameters[14];
    /* 0x70 */ u16 particles[14];
    /* 0x8C */ u16 effects[14];
    /* 0xA8 */ u16 sounds[14];
} ParticleEvents;

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x18 - 0x4];
    /* 0x18 */ ParticleEvents *events;
} ParticleDesc;

typedef struct {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ void *region;
    /* 0x014 */ u8 pad14[0x18 - 0x14];
    /* 0x018 */ Vec3f velocity;
    /* 0x024 */ u8 pad24[0x40 - 0x24];
    /* 0x040 */ s32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ ParticleDesc *desc;
    /* 0x0FC */ u8 padFC[0x10C - 0xFC];
    /* 0x10C */ void *owner;
    /* 0x110 */ s32 ownerId;
    /* 0x114 */ s32 target;
    /* 0x118 */ u8 pad118[0x168 - 0x118];
    /* 0x168 */ Vec3f rotSpeed;
    /* 0x174 */ u8 pad174[0x190 - 0x174];
    /* 0x190 */ u32 hitFlags;
    /* 0x194 */ u8 pad194;
    /* 0x195 */ s8 dieOnImpact;
    /* 0x196 */ s8 dieOnBounce;
    /* 0x197 */ u8 pad197[0x1A0 - 0x197];
    /* 0x1A0 */ f32 rotationScale;
} Particle;

extern s32 D_800F1DA4;
extern Vec3f D_800F1DA8;
extern s32 D_800F1DB4;
extern void func_00236EA0(Particle *, Particle *, s32 effect, s32, Vec3f pos, IntPair params);
extern void func_0020E810(Quatf *out, Vec3f *dir);          /* rotation facing dir */
extern void func_0022F350(void *sys, Particle *p, void *owner, s32 ownerId, s32 target, s32 type, Vec3f dir,
                          Quatf rot, Vec3f pos, s32 region, s32 arg10, s32 flags);
extern void func_00275544(s16 sound, Vec3f pos, s32, s32);
extern void func_0020EF60(Vec3f *out, Vec3f *in, f32 s);    /* out = in * s */
extern void func_0022F1C8(void *sys, Particle *p);
extern void func_00232B40(Particle *p);
extern Vec3f D_800F1DB8;
extern u8 D_800F8928[];

void func_0022BD10(Particle *arg0) {
    IntPair pair;
    Quatf rotation;
    Vec3f position;
    s32 surface;
    s32 index;
    ParticleEvents *events;
    s32 particle;
    s32 effect;
    s32 sound;
    ParticleDesc *desc;

    switch (D_800F1DB4) {
    case 1:
    default:
        surface = 1;
        break;
    case 7:
        surface = 7;
        break;
    case 8:
        surface = 8;
        break;
    }
    arg0->flags ^= 4;
    desc = arg0->desc;
    index = surface;
    events = desc->events;
    particle = events->particles[index];
    effect = events->effects[index];
    pair = events->parameters[surface];
    sound = desc->events->sounds[index];
    if (effect != 0xFFFF) {
        func_00236EA0(arg0, arg0, effect, -1, D_800F1DA8, pair);
    }
    if (particle != 0xFFFF) {
        if ((arg0->desc->flags & 0x10) != 0) {
            position = D_800F1DB8;
        } else {
            position = arg0->velocity;
        }
        func_0020E810(&rotation, &position);
        func_0022F350(D_800F8928, arg0, arg0->owner, arg0->ownerId, arg0->target, particle, position,
                      rotation, D_800F1DA8, (arg0->hitFlags & 0x1000000) ? 0 : D_800F1DA4, -5, (arg0->flags & 0x200006) | 1);
    }
    if (sound != 0xFFFF) {
        func_00275544((s16)sound, D_800F1DA8, 0, -1);
    }
    arg0->flags |= 0x200;
    if ((arg0->desc->flags & 0x20000) != 0) {
        func_0022F1C8(D_800F8928, arg0);
        func_00232B40(arg0);
    }
}
