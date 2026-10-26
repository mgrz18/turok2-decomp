/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_3/80279208_11.CPP, func_80279B40_de, with Turok 2's layouts. */
#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;
typedef struct { f32 x, y, z, w; } Quatf;
typedef f32 Mtx4f[4][4];

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ s32 noOffset;
} ParticleDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ void *region;
    /* 0x014 */ u8 pad14[0x18 - 0x14];
    /* 0x018 */ Vec3f dir;
    /* 0x024 */ u8 pad24[0x40 - 0x24];
    /* 0x040 */ s32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ ParticleDef *def;
    /* 0x0FC */ u8 padFC[0x10C - 0xFC];
    /* 0x10C */ void *owner;
    /* 0x110 */ s32 ownerId;
    /* 0x114 */ s32 target;
    /* 0x118 */ u8 pad118[0x15C - 0x118];
    /* 0x15C */ f32 offsetX;
    /* 0x160 */ f32 offsetY;
    /* 0x164 */ s32 angle;
} Particle;

extern void func_0020E810(Quatf *out, Vec3f *dir);          /* rotation facing dir */
extern void func_00210630(Mtx4f m, s32 angle);              /* rotation about Y */
extern void func_0020F85C(Mtx4f m, Vec3f *in, Vec3f *out);  /* transform */
extern void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);  /* out = a + b */
extern Mtx4f *func_0022CF40(Particle *p, s32 arg1);         /* parent transform */
extern void func_002688C8(Particle *p, Vec3f to, Vec3f *outPos, void **outRegion, s32, s32);
extern void func_0022F350(void *sys, Particle *p, void *owner, s32 ownerId, s32 target, s32 model, Vec3f dir,
                          Quatf rot, Vec3f pos, void *region, s32 arg2, s32 flags);
extern Vec3f D_800F1DB8;
extern void *D_800F1ADC;
extern u8 D_800F1CE0[];
extern u8 D_800F8928[];

static __inline__ f32 offset_x(Particle *p) {
    if (p->def->noOffset != 0) {
        return 0.0f;
    }
    return p->offsetX;
}

static __inline__ f32 offset_y(Particle *p) {
    if (p->def->noOffset != 0) {
        return 0.0f;
    }
    return p->offsetY;
}

void func_00229340(Particle *p, s32 model, s32 arg2, s32 flags) {
    Quatf rotation;
    Vec3f direction;
    Vec3f offset;
    Vec3f turned;
    Vec3f carried;
    Vec3f point;
    Vec3f moved;
    Mtx4f matrix;
    u8 scratch[0xF0];
    void *region;
    Mtx4f *parent;

    if (p->def->flags & 0x10) {
        direction = D_800F1DB8;
    } else {
        direction = p->dir;
    }
    func_0020E810(&rotation, &direction);
    if (offset_x(p) == 0.0f && offset_y(p) == 0.0f) {
        point = p->pos;
        region = p->region;
    } else {
        offset.x = -offset_x(p);
        offset.y = -offset_y(p);
        offset.z = 0.0f;
        func_00210630(matrix, p->angle);
        func_0020F85C(matrix, &offset, &turned);
        parent = func_0022CF40(p, 0);
        if (parent != NULL) {
            func_0020F85C(*parent, &turned, &carried);
        } else {
            carried = turned;
        }
        func_0020EEF8(&moved, &carried, &p->pos);
        D_800F1ADC = scratch;
        if (p->region != NULL) {
            func_002688C8(p, moved, &point, &region, 0, 0);
        } else {
            point = moved;
            region = NULL;
        }
        D_800F1ADC = D_800F1CE0;
    }
    func_0022F350(D_800F8928, p, p->owner, p->ownerId, p->target, model, direction, rotation, point, region,
                  arg2, flags | (p->flags & 0x200006));
}
