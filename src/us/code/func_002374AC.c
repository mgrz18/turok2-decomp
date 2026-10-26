#include "common.h"

typedef struct Vec3f { f32 x, y, z; } Vec3f;
typedef struct Quat { f32 x, y, z, w; } Quat;
typedef struct Mtx44f { f32 m[4][4]; } Mtx44f;

/* A blast: radius, damage, then the push (strength, x, y, z). */
typedef struct Blast {
    u16 radius;
    u16 damage;
    s8 strength;
    s8 x;
    s8 y;
    s8 z;
} __attribute__((aligned(4))) Blast;

typedef struct PushOwner {
    /* 0x000 */ u8 pad0[0xBB0];
    /* 0xBB0 */ s32 reduced;
} PushOwner;

typedef struct PushSource {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[0xD4 - 0x10];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x1A8 - 0xD8];
    /* 0x1A8 */ PushOwner *owner;
} PushSource;

typedef struct PushTarget {
    /* 0x000 */ u8 kind;            /* 1: player, 2: object */
    /* 0x001 */ u8 pad1;
    /* 0x002 */ u16 type;
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[0x8];
    /* 0x018 */ Vec3f dir;
    /* 0x024 */ u8 pad24[0x50 - 0x24];
    /* 0x050 */ f32 angle;
    /* 0x054 */ u8 pad54[0x17C - 0x54];
    /* 0x17C */ u32 playerFlags;
} PushTarget;

extern s32 D_8011001C;

void func_0020E810(Quat *out, Vec3f *dir);
void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);
void func_0020EF60(Vec3f *out, Vec3f *in, f32 scale);
void func_0020F79C(Mtx44f *m);
void func_0020FAFC(Mtx44f *m, Vec3f *in, Vec3f *out);
void func_002105A4(Mtx44f *m, f32 angle);
void func_00211114(Quat *q, Mtx44f *m);

/* The blast's push from `src` turned into a world vector, in the frame of `target`. */
void func_002374AC(PushSource *src, PushTarget *target, Blast blast, Vec3f *out, u32 flags) {
    Vec3f v;
    Vec3f dir;
    Quat q;
    Mtx44f m;
    PushTarget *obj;

    if (D_8011001C != 0) {
        out->x = 0.0f;
        out->y = 0.0f;
        out->z = 0.0f;
        return;
    }
    v.x = blast.x;
    v.y = blast.y;
    v.z = blast.z;
    func_0020EF60(&v, &v, blast.strength * 0.80629915f);
    switch (target->kind) {
    case 1:
        if (target->playerFlags & 0x2000) {
            v.x = 0.0f;
            v.z = 0.0f;
            v.y = 133.12f;
        }
        func_002105A4(&m, target->angle + 3.1415927f);
        break;
    case 2:
        obj = target;
        switch (obj->type) {
        case 0xE:
            dir.x = 0.0f;
            dir.z = 0.0f;
            v.x = 0.0f;
            v.y = 0.0f;
            dir.y = 1.0f;
            v.z = 512.0f;
            break;
        case 0x68:
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = 409.59998f;
            dir = obj->dir;
            dir.y = 0.0f;
            break;
        default:
            if (flags & 0x400000) {
                func_0020EF2C(&dir, &src->pos, &obj->pos);
                dir.y = 7.68f;
            } else {
                dir.x = 0.0f;
                dir.y = 0.0f;
                dir.z = 1.0f;
            }
            break;
        }
        func_0020E810(&q, &dir);
        func_00211114(&q, &m);
        break;
    default:
        func_0020F79C(&m);
        break;
    }
    if (src->flags & 0x300000) {
        if (src->owner->reduced != 0) {
            func_0020EF60(&v, &v, 0.1f);
        }
    }
    func_0020FAFC(&m, &v, out);
}
