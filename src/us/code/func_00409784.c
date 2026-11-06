#include "common.h"
#include "structs/vector.h"
#include "structs/level_actor.h"
#include "structs/boss.h"
#include "globals/scene.h"

/* Boss fight tick: updates the 15 attack slots, then (outside cinematics)
 * counts up the two timers: by phase, drops a random pickup somewhere in the
 * arena (inlined MotherBoss_SpawnPickup) and, later, a pickup at every spawn point
 * (MotherBoss_DropPickupsAtSpawnPoints). */

extern s32 gMotherBoss_minRadius;      /* arena radius range */
extern s32 gMotherBoss_maxRadius;
extern u8 sMotherBossPickupTable[];     /* pickup weights */
extern u8 sMotherBossDropTablePhase1[];
extern u8 sMotherBossDropTablePhase2[];
extern u8 gScene_m_ActorPool[];

extern void Animator_Update(void *slot);
extern s32 Cinema_PathPlaying(void);
extern s32 WeightedTable_PickStatic(void *table);
extern f32 Random_Blend(f32 lo, f32 hi);
extern f32 func_002982F0(f32);
extern f32 func_002974C0(f32);
extern s32 CScene__FindRegionAt(void *level, CVector3 *pos);
extern CPooledLevelActor *CLevelActorPool__Spawn(void *pool, s32 *refCount, s32 id, CVector3 vel, CVector3 pos, s32 region, f32 lifetime);
extern void Actor_SpawnParticle(CPooledLevelActor *actor, s32 sound, CVector3 pos, s32 arg);
extern void MotherBoss_DropPickupsAtSpawnPoints(s32 lifetime, void *table);

/* MotherBoss_SpawnPickup */
static __inline__ void spawnPickup(s32 id, s32 lifetime, CVector3 pos) {
    CVector3 vel;
    s32 region;
    CPooledLevelActor *pickup;

    region = CScene__FindRegionAt(&gScene, &pos);
    vel.x = 0.0f;
    vel.y = 512.0f;
    vel.z = 0.0f;
    pickup = CLevelActorPool__Spawn(gScene_m_ActorPool, NULL, id, vel, pos, region, lifetime);
    if (pickup != NULL) {
        Actor_SpawnParticle(pickup, 0x11D, pos, 0);
    }
}

void MotherBoss_UpdateSpotsAndPickups(MotherBossState *s) {
    CVector3 pos;
    s32 i;
    s32 spawn;
    s32 lifetime;
    s32 drop;
    s32 dropLifetime;
    void *table;
    s32 id;
    f32 dist;
    f32 angle;

    spawn = 0;
    drop = 0;
    lifetime = 0;
    dropLifetime = 0;
    table = NULL;
    for (i = 0; i < 15; i++) {
        Animator_Update(&s->slots[i].anim);
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    s->pickupTimer++;
    s->dropTimer++;
    switch (s->phase) {
    case 1:
        if (s->pickupTimer >= 76) {
            spawn = 1;
            lifetime = 600;
        }
        if (s->dropTimer >= 526) {
            drop = 1;
            table = sMotherBossDropTablePhase1;
            dropLifetime = 330;
        }
        break;
    case 2:
        if (s->pickupTimer >= 151) {
            spawn = 1;
            lifetime = 450;
        }
        if (s->dropTimer >= 526) {
            drop = 1;
            table = sMotherBossDropTablePhase2;
            dropLifetime = 330;
        }
        break;
    case 0:
        if (s->pickupTimer >= 151) {
            spawn = 1;
            lifetime = 450;
        }
        break;
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    if (spawn) {
        s->pickupTimer = 0;
        id = (s16) WeightedTable_PickStatic(sMotherBossPickupTable);
        dist = Random_Blend(gMotherBoss_minRadius, gMotherBoss_maxRadius);
        angle = Random_Blend(-3.1415927f, 3.1415927f);
        pos.x = dist * func_002982F0(angle);
        pos.z = dist * func_002974C0(angle);
        pos.y = 0.0f;
        spawnPickup(id, lifetime, pos);
    }
    if (table != NULL && drop) {
        s->dropTimer = 0;
        MotherBoss_DropPickupsAtSpawnPoints(dropLifetime, table);
    }
}
