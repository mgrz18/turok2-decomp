#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/game.h"



extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */
extern void GameObject_TraceMove(CGameObjectInstance *actor, CVector3 pos, CVector3 *outPos, s32 *outRegion, s32 flagA, s32 flagB);

/* Spawns an actor at `dist` from `origin` in direction `angle`, if the budget allows it. */
void BlindOne_SpawnMinion(CGameObjectInstance **self, s32 *budget, s32 *spawned, s32 max, void *intelligence, CGameObjectInstance *origin, f32 dist,
                   f32 angle) {
    CVector3 pos;
    CVector3 vel;
    CVector3 scale;
    sptr region;

    if (budget != NULL && *budget == 0) {
        return;
    }
    if (spawned != NULL && max != -1 && *spawned >= max) {
        return;
    }
    vel.x = 0.0f;
    vel.y = 0.0f;
    vel.z = 0.0f;
    scale.x = 0.5f;
    scale.y = 0.5f;
    scale.z = 0.5f;
    pos.x = origin->header.m_vPos.x + dist * func_002982F0(angle);
    pos.y = 0.0f;
    pos.z = origin->header.m_vPos.z + dist * func_002974C0(angle);
    GameObject_TraceMove(*self, pos, &pos, &region, 0, 0);
    if (CInstancePool__Spawn(&gScene.m_InstancePool, -1, 0x1FF, intelligence, region, pos, angle, vel, scale, spawned) != NULL &&
        budget != NULL) {
        (*budget)--;
    }
}
