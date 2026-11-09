#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/scene.h"
#include "protos/scene.h"

/* Spawns one actor near each of the four spawn points, scattered by up to +-15 units, skipping a point
 * closer than 460.8 to the target of the module object. */

extern f32 MotherBoss_GetTargetDist_2(void *obj, CGameObjectInstance **found);
extern s32 WeightedTable_PickStatic(void *a);
extern f32 Random_Blend(f32 min, f32 max);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern f32 func_00298470(f32 x); /* sqrt */
extern void *CLevelActorPool__Spawn(CLevelActorPool *pool, s32 a1, s16 type, CVector3 vel, CVector3 pos, s32 region, f32 arg);
extern void Actor_SpawnParticle(void *actor, s32 id, CVector3 pos, s32 a5);

static __inline__ void SpawnAt(s32 arg0, s16 type, CVector3 pos, f32 speed) {
    CVector3 vel;
    sptr region;
    void *actor;

    region = CScene__FindRegionAt(&gScene, &pos);
    vel.x = 0.0f;
    vel.y = speed;
    vel.z = 0.0f;
    actor = CLevelActorPool__Spawn(&gScene.m_ActorPool, 0, type, vel, pos, region, arg0);
    if (actor != NULL) {
        Actor_SpawnParticle(actor, 0x11D, pos, 0);
    }
}

void MotherBoss_DropPickupsAtSpawnPoints_2(s32 arg0, void *arg1) {
    CVector3 v;
    CGameObjectInstance *found;
    s32 i;
    s32 type;
    s32 dx;
    s32 dz;

    MotherBoss_GetTargetDist_2(gMotherBoss.obj, &found);
    for (i = 0; i < 4; i++) {
        type = (s16)WeightedTable_PickStatic(arg1);
        dx = Random_Blend(-15.0f, 15.0f);
        dz = Random_Blend(-15.0f, 15.0f);
        dx = dx * 10.24f;
        dz = dz * 10.24f;
        if (found != NULL) {
            CVector3__Subtract(&v, &found->header.m_vPos, &gMotherSpawnPoints_2[i].pos);
            if (func_00298470(v.x * v.x + v.y * v.y + v.z * v.z) > 460.8f) {
                v = gMotherSpawnPoints_2[i].pos;
                v.y = 30.72f;
                v.x += dx;
                v.z += dz;
                SpawnAt(arg0, type, v, 512.0f);
            }
        } else {
            v = gMotherSpawnPoints_2[i].pos;
            v.y = 30.72f;
            v.x += dx;
            v.z += dz;
            SpawnAt(arg0, type, v, 512.0f);
        }
    }
}
