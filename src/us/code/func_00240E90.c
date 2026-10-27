#include "common.h"

typedef struct SweepDef {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s8 kind;
    /* 0x05 */ s8 hitsMap;
} SweepDef;

typedef struct SweepInst {
    /* 0x00 */ u8 type;
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 z;
    /* 0x10 */ void *region;
    /* 0x14 */ u8 pad14[0xC0];
    /* 0xD4 */ s32 flags;
} SweepInst;

typedef struct Sweep {
    /* 0x00 */ SweepInst *inst;
    /* 0x04 */ SweepDef *def;
    /* 0x08 */ u8 pad8[0x44];
    /* 0x4C */ f32 endX;
    /* 0x50 */ f32 endY;
    /* 0x54 */ f32 endZ;
} Sweep;

/* The state of a sweep against the map, read by the region walkers. */
typedef struct MapSweep {
    /* 0x00 */ Sweep *sweep;
    /* 0x04 */ u8 pad4[0x30];
    /* 0x34 */ f32 unk34;
    /* 0x38 */ u8 pad38[8];
    /* 0x40 */ f32 unk40;
    /* 0x44 */ u8 pad44[0x10];
    /* 0x54 */ f32 radius;
    /* 0x58 */ u8 pad58[0x1C];
    /* 0x74 */ f32 minX;
    /* 0x78 */ f32 minZ;
    /* 0x7C */ f32 maxX;
    /* 0x80 */ f32 maxZ;
    /* 0x84 */ u8 pad84[4];
} MapSweep;

extern MapSweep *D_800F1AB0;

f32 func_00268460(SweepInst *inst);
void func_00240800(void *region);
void func_0026CC0C(void *region);

/* Sweep an instance against the map, starting from its region. */
void func_00240E90(Sweep *sweep) {
    MapSweep ms;
    SweepDef *def;
    SweepInst *inst;
    void *region;
    f32 lo;
    f32 hi;

    def = sweep->def;
    if (!(def->flags & 2) || def->hitsMap == 0 || func_00268460(sweep->inst) == 0.0f) {
        return;
    }
    inst = sweep->inst;
    region = inst->region;
    if (region == NULL) {
        return;
    }
    if (inst->type == 1 && !(inst->flags & 0x300200)) {
        return;
    }
    D_800F1AB0 = &ms;
    D_800F1AB0->radius = func_00268460(inst);
    D_800F1AB0->sweep = sweep;
    D_800F1AB0->unk40 = 0.0f;
    D_800F1AB0->unk34 = 0.0f;
    if (sweep->inst->x < sweep->endX) {
        lo = sweep->inst->x;
        hi = sweep->endX;
    } else {
        lo = sweep->endX;
        hi = sweep->inst->x;
    }
    D_800F1AB0->minX = lo - D_800F1AB0->radius;
    D_800F1AB0->maxX = hi + D_800F1AB0->radius;
    if (sweep->inst->z < sweep->endZ) {
        lo = sweep->inst->z;
        hi = sweep->endZ;
    } else {
        lo = sweep->endZ;
        hi = sweep->inst->z;
    }
    D_800F1AB0->minZ = lo - D_800F1AB0->radius;
    D_800F1AB0->maxZ = hi + D_800F1AB0->radius;
    func_00240800(region);
    func_0026CC0C(region);
}
