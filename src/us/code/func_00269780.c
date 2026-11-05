#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"

typedef struct CollisionInfo {
    /* 0x00 */ s32 flags;
} CollisionInfo;

extern CollisionInfo D_800F1AE0;

f32 func_002671B4(CGameObjectInstance *actor);    /* height */
s32 func_00239F00(CGameObjectInstance *actor, CVector3 pos, CollisionInfo *info);

/* Whether actor can see target: a clear line from 80% of one's height to 80% of the other's. */
s32 func_00269780(CGameObjectInstance *actor, CGameObjectInstance *target) {
    CObjectHead saved;
    CVector3 to;
    s32 hit;

    saved = *(CObjectHead *) actor;
    actor->header.m_vPos.y += func_002671B4(actor) * 0.8f;
    to = target->header.m_vPos;
    to.y += func_002671B4(target) * 0.8f;
    hit = func_00239F00(actor, to, &D_800F1AE0);
    *(CObjectHead *) actor = saved;
    return hit == 0;
}
