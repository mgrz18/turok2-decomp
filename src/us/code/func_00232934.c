#include "common.h"

typedef struct { f32 x, y, z; } Point3;
typedef struct { f32 x, y, z, w; } Quat4;

typedef struct {
    u8 pad0[0xC];
    /* 0xC */ s8 count;
} TrailInfo;

typedef struct {
    u8 pad0[0x38];
    /* 0x38 */ TrailInfo *trail;
} ActorDef;

typedef struct {
    u8 pad0[0x2];
    /* 0x002 */ u16 typeId;
    /* 0x004 */ Point3 pos;
    /* 0x010 */ s32 arg;
    u8 pad14[0x4];
    /* 0x018 */ Point3 dir;
    u8 pad24[0xD4];
    /* 0x0F8 */ ActorDef *def;
    u8 padFC[0x10];
    /* 0x10C */ void *parent;
    /* 0x110 */ void *slot;
    /* 0x114 */ s32 a;
    u8 pad118[0x2C];
    /* 0x144 */ Point3 prevPos;
} Actor;

extern u8 D_800F8928[];
void func_0020E810(Quat4 *out, Point3 *dir);
void func_0020EF2C(Point3 *out, Point3 *a, Point3 *b);
void func_0020EF60(Point3 *out, Point3 *in, f32 scale);
void func_0020EEF8(Point3 *out, Point3 *a, Point3 *b);
s32 func_0022F350(void *table, void *actor, void *parent, void *slot, s32 a, s32 kind, Point3 origin,
                  Quat4 rot, Point3 pos, s32 arg, s32 b, s32 c);

/* Spawns `count` copies along the segment the actor covered this frame. */
void func_00232934(Actor *actor) {
    Quat4 rot;
    Point3 delta;
    Point3 pos;
    Point3 dir;
    s32 count;
    s32 kind;
    s32 i;
    u8 *table;

    count = actor->def->trail->count;
    if (count > 0) {
        dir = actor->dir;
        func_0020E810(&rot, &dir);
        func_0020EF2C(&delta, &actor->pos, &actor->prevPos);
        table = D_800F8928;
        for (i = 1; i <= count; i++) {
            func_0020EF60(&pos, &delta, i / (count + 1.0f));
            func_0020EEF8(&pos, &pos, &actor->prevPos);
            kind = actor->typeId;
            func_0022F350(table, actor, actor->parent, actor->slot, actor->a, kind, dir, rot,
                          pos, actor->arg, -1, 0x20E001);
        }
    }
}
