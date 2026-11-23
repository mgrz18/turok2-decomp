#include "common.h"
#include "structs/pick.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/level_actor.h"
#include "globals/boss.h"
#include "globals/scene.h"
#include "protos/scene.h"

/* Spawns a pickup picked from a weighted table at a random point around the origin (MotherBoss_RandomArenaPoint and
 * MotherBoss_SpawnPickup inlined). */

extern s16 WeightedTable_PickStatic(Pick *table);
extern f32 Random_Blend(f32 min, f32 max);
extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */
extern CPooledLevelActor *CLevelActorPool__Spawn(CLevelActorPool *pool, s32 *refCount, s32 id, CVector3 vel, CVector3 pos, sptr region, f32 lifetime);
extern void Actor_SpawnParticle(CPooledLevelActor *actor, s32 sound, CVector3 pos, s32 arg);

/* MotherBoss_RandomArenaPoint */
static __inline__ void randomGroundPoint(CVector3 *out) {
    MotherBossState *state = &gMotherBoss;
    f32 dist;
    f32 angle;
    f32 z;

    dist = Random_Blend((f32)state->minRadius, (f32)state->maxRadius);
    angle = Random_Blend(-3.1415927f, 3.1415927f);
    out->x = dist * func_002982F0(angle);
    z = dist * func_002974C0(angle);
    out->y = 0.0f;
    out->z = z;
}

/* MotherBoss_SpawnPickup */
static __inline__ void spawnPickup(s32 id, s32 lifetime, CVector3 pos) {
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

void MotherBoss_SpawnRandomPickup(s32 lifetime, Pick *table) {
    CVector3 pos;
    s32 id;

    id = WeightedTable_PickStatic(table);
    randomGroundPoint(&pos);
    spawnPickup(id, lifetime, pos);
}
