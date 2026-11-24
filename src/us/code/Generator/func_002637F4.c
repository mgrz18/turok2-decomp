/* The out-of-line copy of func_00262D74.c's spawnPickup. */
#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "structs/level_actor.h"
#include "globals/scene.h"

void *CLevelActorPool__Spawn(CLevelActorPool *pool, void *refCount, s32 id, CVector3 vel, CVector3 pos, sptr region, f32 lifetime);
void Actor_SpawnParticle(void *actor, s32 event, CVector3 pos, s32 arg);

void Generator_SpawnPickup(CGameObjectInstance *actor, CAI *ai, CVector3 pos, sptr region, s32 id) {
    CPooledLevelActor *pickup;

    pickup = CLevelActorPool__Spawn(&gScene.m_ActorPool, &ai->m_Generator.m_nActive, id, actor->m_vVelocity, pos, region, 0.0f);
    if (pickup != NULL) {
        ai->m_Generator.m_nRemaining -= 1;
        Actor_SpawnParticle(pickup, 0x11D, pickup->m_Actor.header.m_vPos, 0);
    }
}
