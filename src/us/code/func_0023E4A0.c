#include "common.h"

typedef struct SweepDef {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s8 active;
} SweepDef;

typedef struct SweepInst {
    /* 0x000 */ u8 type;
    /* 0x004 */ f32 x;
    /* 0x008 */ f32 y;
    /* 0x00C */ f32 z;
    /* 0x010 */ u8 pad10[0x134];
    /* 0x144 */ s32 health;
} SweepInst;

/* A sweep from an instance's position to an end point. */
typedef struct Sweep {
    /* 0x00 */ SweepInst *inst;
    /* 0x04 */ SweepDef *def;
    /* 0x08 */ u8 pad8[0x44];
    /* 0x4C */ f32 endX;
    /* 0x50 */ f32 endY;
    /* 0x54 */ f32 endZ;
} Sweep;

/* The bounds of the volume the instance sweeps. */
typedef struct SweepBox {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 bottom;
    /* 0x08 */ f32 top;
    /* 0x0C */ f32 startTop;
    /* 0x10 */ f32 startBottom;
    /* 0x14 */ f32 endTop;
    /* 0x18 */ f32 endBottom;
    /* 0x1C */ f32 minX;
    /* 0x20 */ f32 minZ;
    /* 0x24 */ f32 maxX;
    /* 0x28 */ f32 maxZ;
    /* 0x2C */ s32 flag;
} SweepBox;

typedef struct World {
    /* 0x000 */ u8 pad0[0xEC];
    /* 0x0EC */ SweepInst *draw[0x200];
    /* 0x8EC */ s32 drawCount;
} World;

extern World D_800F7078;
extern s32 D_800F7964;
extern SweepDef D_800F1C60;

f32 func_002671B4(SweepInst *inst);
f32 func_002672C8(SweepInst *inst);
f32 func_0026841C(SweepInst *inst);
void func_0023DF70(Sweep *sweep, SweepBox *box, SweepInst *other);

#define MIN(a, b) ({ f32 _m = (a); if (!(_m <= (b))) _m = (b); _m; })
#define MAX(a, b) ({ f32 _m = (a); if (!((b) <= _m)) _m = (b); _m; })

/* Sweep an instance's volume against every drawn instance it may hit. */
void func_0023E4A0(Sweep *sweep) {
    SweepBox box;
    SweepBox *b;
    SweepInst *inst;
    SweepDef *def;
    SweepInst *other;
    World *world;
    s32 count;
    f32 x;
    f32 z;
    s32 i;

    def = sweep->def;
    if (def->active == 0 || (def->flags & 0x40000000) || def == &D_800F1C60) {
        return;
    }
    inst = sweep->inst;
    count = D_800F7964; /* D_800F7078.drawCount */
    b = &box;
    box.radius = func_002672C8(inst);
    b->flag = def->flags & 0x100000;
    b->bottom = func_0026841C(inst);
    b->top = b->bottom + func_002671B4(inst);
    b->startBottom = inst->y + b->bottom;
    b->startTop = inst->y + b->top;
    b->endBottom = sweep->endY + b->bottom;
    b->endTop = sweep->endY + b->top;
    x = inst->x;
    z = inst->z;
    b->minX = MIN(sweep->endX, x) - box.radius;
    b->maxX = MAX(sweep->endX, x) + box.radius;
    b->minZ = MIN(sweep->endZ, z) - box.radius;
    b->maxZ = MAX(sweep->endZ, z) + box.radius;
    world = &D_800F7078;
    for (i = 0; i < count; i++) {
        other = world->draw[i];
        if (other == inst || def->active == 0) {
            continue;
        }
        switch (other->type) {
            case 1:
                if ((def->flags & 1) && other->health == 0) {
                    continue;
                }
                break;
            case 2:
                break;
            case 3:
                if (!(def->flags & 0x20)) {
                    continue;
                }
                break;
        }
        func_0023DF70(sweep, &box, other);
    }
}
