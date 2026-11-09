#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/scene.h"
#include "protos/game.h"

typedef CVector3 Vec3_004028F4;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3_004028F4 rot;
} Obj004028F4;

s16 WeightedTable_PickStatic(struct Pick *table);
void GameObject_TraceMove(Obj004028F4 *obj, Vec3_004028F4 pos, Vec3_004028F4 *posp, s32 *out, s32 a5, s32 a6);

void MantidQueen_SpawnMinion(s32 a0, Obj004028F4 *obj, s32 a2, Vec3_004028F4 pos) {
    WaveBossState *s = &gMantidQueen;
    s32 index;
    s32 region;

    index = WeightedTable_PickStatic(s->step->partPicks);
    GameObject_TraceMove(obj, pos, &pos, &region, 0, 0);
    CInstancePool__Spawn(&gScene.m_InstancePool, -1, sMantidQueenMinions[index].model, s->intel[index], region, pos, 0, obj->rot,
                  sMantidQueenMinions[index].offset, 0);
}
