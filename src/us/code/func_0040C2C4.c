#include "common.h"
#include "structs/vector.h"
#include "structs/level_actor.h"
#include "globals/scene.h"
#include "protos/scene.h"

/* Spawns pool pickup id at pos, thrown upward, lasting the given time, and plays its spawn sound. */

extern CPooledLevelActor *CLevelActorPool__Spawn(CLevelActorPool *pool, s32 *refCount, s32 id, CVector3 vel, CVector3 pos, sptr region, f32 lifetime);
extern void Actor_SpawnParticle(CPooledLevelActor *actor, s32 sound, CVector3 pos, s32 arg);

void MotherBoss_SpawnPickup(s32 id, s32 lifetime, CVector3 pos) {
    CVector3 vel;
    sptr region;
    CPooledLevelActor *pickup;

    region = CScene__FindRegionAt(&gScene, &pos);
    vel.x = 0.0f;
    vel.y = 512.0f;
    vel.z = 0.0f;
    pickup = CLevelActorPool__Spawn(&gScene.m_ActorPool, NULL, id, vel, pos, region, lifetime);
    if (pickup != NULL) {
        Actor_SpawnParticle(pickup, 0x11D, pos, 0);
    }
}
