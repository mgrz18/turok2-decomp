#include "common.h"
#include "structs/vector.h"
#include "globals/scene.h"

#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define CLevelActorPool__Spawn CLevelActorPool__Spawn_i
#endif
u8 *CLevelActorPool__Spawn(CLevelActorPool *table, void *slot, s32 arg, CVector3 origin, CVector3 pos, sptr b, s32 c);
void Actor_SpawnParticle(void *actor, s32 event, CVector3 pos, s32 n);

void Generator_SpawnPickup(u8 *actor, u8 *state, CVector3 pos, s32 arg, s32 kind) {
    u8 *spawned;

    spawned = CLevelActorPool__Spawn(&gScene.m_ActorPool, state + 0x10C, kind, *(CVector3 *) (actor + 0x18), pos, arg, 0);
    if (spawned != NULL) {
        *(s32 *) (state + 0x110) -= 1;
        Actor_SpawnParticle(spawned, 0x11D, *(CVector3 *) (spawned + 0x4), 0);
    }
}
