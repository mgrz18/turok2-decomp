#include "common.h"
#include "structs/boss.h"

typedef struct {
    f32 x, y, z;
} Vec3_004028F4;

typedef struct {
    /* 0x00 */ s32 model;
    /* 0x04 */ Vec3_004028F4 offset;
    /* 0x10 */ s32 unk10;
} Part004028F4;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3_004028F4 rot;
} Obj004028F4;

extern u8 gScene_m_InstancePool[];
extern Part004028F4 sMantidQueenMinions_model[];
s16 WeightedTable_PickStatic(struct Pick *table);
void GameObject_TraceMove(Obj004028F4 *obj, Vec3_004028F4 pos, Vec3_004028F4 *posp, s32 *out, s32 a5, s32 a6);
void CInstancePool__Spawn(void *a0, s32 a1, s32 model, void *intel, s32 region, Vec3_004028F4 pos, s32 a8,
                   Vec3_004028F4 rot, Vec3_004028F4 offset, s32 a9);

void MantidQueen_SpawnMinion(s32 a0, Obj004028F4 *obj, s32 a2, Vec3_004028F4 pos) {
    WaveBossState *s = &gMantidQueen;
    s32 index;
    s32 region;

    index = WeightedTable_PickStatic(s->step->partPicks);
    GameObject_TraceMove(obj, pos, &pos, &region, 0, 0);
    CInstancePool__Spawn(gScene_m_InstancePool, -1, sMantidQueenMinions_model[index].model, s->intel[index], region, pos, 0, obj->rot,
                  sMantidQueenMinions_model[index].offset, 0);
}
