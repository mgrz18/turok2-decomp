#include "common.h"
#include "structs/vector.h"
#include "structs/boss.h"
#include "globals/scene.h"

/* Module update: ticks every slot, then (outside cinematics) counts the two phase timers: the first spawns
 * one actor at a random spot around the origin, the second starts a burst of spawns at the phase points. */

extern s32 gMotherBoss_minRadius; /* gMotherBoss.minRadius */
extern s32 gMotherBoss_maxRadius; /* gMotherBoss.maxRadius */
extern u8 sMotherBossPickupTable_2[];
extern u8 sMotherBossDropTablePhase1_2[];
extern u8 sMotherBossDropTablePhase2_2[];
extern void Animator_Update(void *slot);
extern s32 Cinema_PathPlaying(void);
extern s32 WeightedTable_PickStatic(void *a);
extern f32 Random_Blend(f32 min, f32 max);
extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */
extern s32 CScene__FindRegionAt(void *world, CVector3 *pos);
extern void *CLevelActorPool__Spawn(void *pool, s32 a1, s16 type, CVector3 vel, CVector3 pos, s32 region, f32 arg);
extern void Actor_SpawnParticle(void *actor, s32 id, CVector3 pos, s32 a5);
extern void MotherBoss_DropPickupsAtSpawnPoints_2(s32 delay, void *points);

static __inline__ void SpawnAt(s32 delay, s16 type, CVector3 pos, f32 speed) {
    CVector3 vel;
    s32 region;
    void *actor;

    region = CScene__FindRegionAt(&gScene, &pos);
    vel.x = 0.0f;
    vel.y = speed;
    vel.z = 0.0f;
    actor = CLevelActorPool__Spawn(gScene.m_ActorPool, 0, type, vel, pos, region, delay);
    if (actor != NULL) {
        Actor_SpawnParticle(actor, 0x11D, pos, 0);
    }
}

void MotherBoss_UpdateSpotsAndPickups_2(MotherBossState *state) {
    CVector3 v;
    s32 i;
    s32 spawn = 0;
    s32 burst = 0;
    s32 delay = 0;
    s32 count = 0;
    void *points = NULL;
    s32 type;
    f32 r;
    f32 angle;

    for (i = 0; i < 15; i++) {
        Animator_Update(&state->slots[i].anim);
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    state->pickupTimer++;
    state->dropTimer++;
    switch (state->phase) {
        case 1:
            if (state->pickupTimer >= 76) {
                spawn = 1;
                delay = 600;
            }
            if (state->dropTimer >= 526) {
                burst = 1;
                points = sMotherBossDropTablePhase1_2;
                count = 330;
            }
            break;
        case 2:
            if (state->pickupTimer >= 151) {
                spawn = 1;
                delay = 450;
            }
            if (state->dropTimer >= 526) {
                burst = 1;
                points = sMotherBossDropTablePhase2_2;
                count = 330;
            }
            break;
        case 0:
            if (state->pickupTimer >= 151) {
                spawn = 1;
                delay = 450;
            }
            break;
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    if (spawn) {
        state->pickupTimer = 0;
        type = (s16)WeightedTable_PickStatic(sMotherBossPickupTable_2);
        r = Random_Blend(gMotherBoss_minRadius, gMotherBoss_maxRadius);
        angle = Random_Blend(-3.1415927f, 3.1415927f);
        v.x = r * func_002982F0(angle);
        v.z = r * func_002974C0(angle);
        v.y = 0.0f;
        SpawnAt(delay, type, v, 512.0f);
    }
    if (points != NULL && burst) {
        state->dropTimer = 0;
        MotherBoss_DropPickupsAtSpawnPoints_2(count, points);
    }
}
