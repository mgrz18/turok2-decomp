#include "common.h"
#include "structs/vector.h"
#include "structs/level_actor.h"

/* Spawns pool pickup id at pos, thrown upward, lasting the given time, and plays its spawn sound. */

extern u8 D_800F7078[];
extern u8 D_80107590[];
extern s32 func_00220A1C(void *level, CVector3 *pos);
extern CPooledLevelActor *func_00227240(void *pool, s32 *refCount, s32 id, CVector3 vel, CVector3 pos, s32 region, f32 lifetime);
extern void func_00246690(CPooledLevelActor *actor, s32 sound, CVector3 pos, s32 arg);

void func_0040C2C4(s32 id, s32 lifetime, CVector3 pos) {
    CVector3 vel;
    s32 region;
    CPooledLevelActor *pickup;

    region = func_00220A1C(D_800F7078, &pos);
    vel.x = 0.0f;
    vel.y = 512.0f;
    vel.z = 0.0f;
    pickup = func_00227240(D_80107590, NULL, id, vel, pos, region, lifetime);
    if (pickup != NULL) {
        func_00246690(pickup, 0x11D, pos, 0);
    }
}
