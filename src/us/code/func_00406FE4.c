#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"

extern void *CScene__FindRegionAt(void *world, CVector3 *pos);                  /* region at a point */
extern void *CLevelActorPool__Spawn(void *table, void *slot, s32 kind, CVector3 velocity, CVector3 pos,
                           void *region, f32 arg);
extern void Actor_SpawnParticle(void *actor, s32 event, CVector3 pos, s32 arg);

extern u8 gScene_m_ActorPool[];

/* Spawn an object of the given kind at pos, thrown upwards, and send it event 0x11D. */
void Primagen_SpawnTossedObject(s32 kind, s32 count, CVector3 pos) {
    CVector3 velocity;
    void *region;
    void *spawned;

    region = CScene__FindRegionAt(&gScene, &pos);
    velocity.x = 0.0f;
    velocity.y = 512.0f;
    velocity.z = 0.0f;
    spawned = CLevelActorPool__Spawn(gScene_m_ActorPool, NULL, kind, velocity, pos, region, count);
    if (spawned != NULL) {
        Actor_SpawnParticle(spawned, 0x11D, pos, 0);
    }
}
