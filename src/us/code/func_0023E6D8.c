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

f32 func_002671B4(SweepInst *inst);
f32 func_002672C8(SweepInst *inst);
f32 func_0026841C(SweepInst *inst);

#define MIN(a, b) ({ f32 _m = (a); if (!(_m <= (b))) _m = (b); _m; })
#define MAX(a, b) ({ f32 _m = (a); if (!((b) <= _m)) _m = (b); _m; })

/* Fill the bounds of the volume a sweep's instance moves through. */
void func_0023E6D8(Sweep *sweep, SweepBox *box) {
    SweepInst *inst;
    SweepDef *def;
    f32 x;
    f32 z;

    inst = sweep->inst;
    def = sweep->def;
    box->radius = func_002672C8(inst);
    box->flag = def->flags & 0x100000;
    box->bottom = func_0026841C(inst);
    box->top = box->bottom + func_002671B4(inst);
    box->startBottom = inst->y + box->bottom;
    box->startTop = inst->y + box->top;
    box->endBottom = sweep->endY + box->bottom;
    box->endTop = sweep->endY + box->top;
    x = inst->x;
    z = inst->z;
    box->minX = MIN(sweep->endX, x) - box->radius;
    box->maxX = MAX(sweep->endX, x) + box->radius;
    box->minZ = MIN(sweep->endZ, z) - box->radius;
    box->maxZ = MAX(sweep->endZ, z) + box->radius;
}
