#include "common.h"

typedef struct { f32 x, y, z; } Point3;

/* The view the spawn is placed from (0x800F1CE8). */
typedef struct {
    /* 0x00 */ Point3 pos;
    u8 padC[0xC4];
    /* 0xD0 */ Point3 dir;
} SpawnView;

typedef struct {
    u8 pad0[0x2];
    /* 0x002 */ u16 typeId;
    /* 0x004 */ Point3 pos;
    u8 pad10[0x8];
    /* 0x018 */ Point3 dir;
    u8 pad24[0x1C];
    /* 0x040 */ u32 flags;
    u8 pad44[0x10C];
    /* 0x150 */ Point3 heading;
    u8 pad15C[0x51];
    /* 0x1AD */ u8 b1AD;
} Actor;

extern SpawnView D_800F1CE8;
void func_0020F040(Point3 *v);
f32 func_002119FC(f32 a, f32 b);
void func_0020EF60(Point3 *out, Point3 *in, f32 scale);
void func_0020EEF8(Point3 *out, Point3 *a, Point3 *b);

void func_00232044(Actor *actor, s32 kind) {
    Point3 dir;

    actor->flags |= 0x20000;
    actor->pos = D_800F1CE8.pos;
    actor->dir = D_800F1CE8.dir;
    dir = D_800F1CE8.dir;
    func_0020F040(&dir);
    actor->heading = dir;
    func_0020EF60(&dir, &dir, func_002119FC(-49.92f, 0.0f));
    func_0020EEF8(&actor->pos, &actor->pos, &dir);
    actor->b1AD = 0x7F;
    if (actor->typeId == 0x56 && kind == 2) {
        actor->flags |= 0x04000000;
    }
}
