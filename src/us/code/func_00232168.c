#include "common.h"

typedef struct { f32 x, y, z; } Point3;
typedef struct { f32 m[4][4]; } Mtx4;

/* The view the spawn is placed from (0x800F1CE8). */
typedef struct {
    /* 0x00 */ Point3 pos;
    u8 padC[0x3];
    /* 0x0F */ u8 b0F;
    u8 pad10[0xC0];
    /* 0xD0 */ Point3 dir;
} SpawnView;

typedef struct {
    /* 0x00 */ u8 *owner;
    u8 pad4[0x10];
    /* 0x14 */ s32 node;
} SpawnSource;

typedef struct {
    u8 pad0[0x18];
    /* 0x018 */ Point3 dir;
    u8 pad24[0x10];
    /* 0x034 */ Point3 pos;
    /* 0x040 */ u32 flags;
    u8 pad44[0xC8];
    /* 0x10C */ u8 *parent;
    u8 pad110[0x4];
    /* 0x114 */ u8 *owner;
    u8 pad118[0x95];
    /* 0x1AD */ u8 b1AD;
} Actor;

extern SpawnSource D_800F1CE0;
extern SpawnView D_800F1CE8;
void func_0020D908(Mtx4 *out, void *src);
void func_0020C208(Mtx4 *out, Mtx4 *in);
void func_0020F85C(Mtx4 *m, Point3 *in, Point3 *out);
void func_0020F040(Point3 *v);
void func_0020FAFC(Mtx4 *m, Point3 *in, Point3 *out);
void func_0025E52C(u8 *owner, u8 *ai, void *pos, s32 n);

void func_00232168(Actor *actor, Mtx4 *mtx, s32 kind) {
    Mtx4 tmp;
    Mtx4 m;
    Point3 dir;
    u8 *owner;

    owner = D_800F1CE0.owner;
    if (mtx != NULL) {
        func_0020C208(&m, mtx);
    } else {
        func_0020D908(&tmp, *(u8 **) (owner + 0x98) + D_800F1CE0.node * 0x40);
        func_0020C208(&m, &tmp);
    }
    func_0020F85C(&m, &D_800F1CE8.pos, &actor->pos);
    dir = D_800F1CE8.dir;
    func_0020F040(&dir);
    func_0020FAFC(&m, &dir, &actor->dir);
    actor->owner = owner;
    actor->flags |= 0x10000;
    actor->b1AD = D_800F1CE8.b0F;
    if (**(s32 **) (owner + 0x14) == 1) {
        if (actor->parent != NULL) {
            func_0025E52C(owner, owner + 0x140, actor->parent + 4, 3);
        } else {
            func_0025E52C(owner, owner + 0x140, &D_800F1CE8, 3);
        }
    }
    if (*(u16 *) ((u8 *) actor + 2) == 0x56 && kind == 2) {
        actor->flags |= 0x04000000;
    }
}
