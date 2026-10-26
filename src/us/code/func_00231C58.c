#include "common.h"

typedef struct { f32 x, y, z; } Point3;
typedef struct { f32 x, y, z, w; } Quat4;

/* Spawn parameters next to each other in .data. */
typedef struct {
    /* 0x0 */ s32 arg;
    /* 0x4 */ Point3 pos;
} SpawnParams;

typedef struct {
    u8 pad0[0x18];
    /* 0x018 */ Point3 origin;
    u8 pad24[0x1C];
    /* 0x040 */ u32 flags;
    u8 pad44[0xB4];
    /* 0x0F8 */ u32 *parentFlags;
    u8 padFC[0x10];
    /* 0x10C */ void *parent;
    /* 0x110 */ void *slot;
    /* 0x114 */ s32 a;
    u8 pad118[0x78];
    /* 0x190 */ u32 flags190;
} Actor;

extern u8 D_800F8928[];
extern SpawnParams D_800F1DA4;
extern Point3 D_800F1DB8;
void func_0020E810(Quat4 *out, Point3 *dir);
s32 func_0022F350(void *table, void *actor, void *parent, void *slot, s32 a, s32 kind, Point3 origin,
                  Quat4 rot, Point3 pos, s32 arg, s32 b, s32 c);

void func_00231C58(Actor *actor, s32 kind) {
    Quat4 rot;
    Point3 origin;

    if (*actor->parentFlags & 0x10) {
        origin = D_800F1DB8;
    } else {
        origin = actor->origin;
    }
    func_0020E810(&rot, &origin);
    func_0022F350(D_800F8928, actor, actor->parent, actor->slot, actor->a, kind, origin, rot,
                  D_800F1DA4.pos, (actor->flags190 & 0x1000000) ? 0 : D_800F1DA4.arg, -5, (actor->flags & 0x200006) | 1);
}
