/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_4.CPP, Particle_ParentTransform, with Turok 2's layouts. */
#include "common.h"

/* The transform a particle's position is relative to, picked by its
 * attachment mode: the particle's own facing, the owner's effect transforms,
 * the region's ground or wall orientation, or a fixed one. */

typedef struct { f32 x, y, z; } Vec3f;
typedef struct { f32 x, y, z, w; } Quatf;
typedef f32 Mtx4f[4][4];

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ void *region;
    /* 0x014 */ u8 pad14[0x150 - 0x14];
    /* 0x150 */ Vec3f dir;
    /* 0x15C */ u8 pad15C[0x1AC - 0x15C];
    /* 0x1AC */ s8 attach;
} Particle;

extern void func_00210090(Mtx4f m, Vec3f *dir);             /* matrix facing dir */
extern f32 func_0026BE60(void *region, f32 x, f32 z);        /* ground height */
extern void func_0026A9C0(Quatf *out, void *region);         /* ground orientation */
extern void func_0026AC38(Quatf *out, void *region);         /* wall orientation */
extern void func_00211114(Quatf *q, Mtx4f m);                /* to matrix */
extern void func_0020C618(Mtx4f out, Mtx4f a, Mtx4f b);      /* out = a * b */
extern f32 func_0026C240(void *region, s32 attach);
extern void func_002105A4(Mtx4f m, f32 angle);
extern Mtx4f D_800F1810;
extern Mtx4f D_800F18F0;

Mtx4f *func_0022CF40(Particle *p, u8 *view) {
    Mtx4f output;
    Quatf state;

    switch (p->attach) {
    case -8:
        func_00210090(D_800F1810, &p->dir);
        return &D_800F1810;
    case -6:
        if (view == NULL) {
            return NULL;
        }
        return (Mtx4f *)(view + 0xE7C);
    case -4:
    case -1:
        if (view == NULL) {
            return NULL;
        }
        return (Mtx4f *)(view + 0xE3C);
    case -3:
        if (p->pos.y - func_0026BE60(p->region, p->pos.x, p->pos.z) <= 0.01f) {
            func_0026A9C0(&state, p->region);
            func_00211114(&state, output);
            func_0020C618(D_800F1810, D_800F18F0, output);
            return &D_800F1810;
        }
    case -5:
        return &D_800F18F0;
    case -2:
        func_0026AC38(&state, p->region);
        func_00211114(&state, output);
        func_0020C618(D_800F1810, D_800F18F0, output);
        return &D_800F1810;
    case 0:
    case 1:
    case 2:
        func_002105A4(D_800F1810, func_0026C240(p->region, p->attach));
        return &D_800F1810;
    }
    return NULL;
}
