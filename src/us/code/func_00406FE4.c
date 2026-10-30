#include "common.h"
#include "structs/game_object.h"

extern void *func_00220A1C(void *world, CVector3 *pos);                  /* region at a point */
extern void *func_00227240(void *table, void *slot, s32 kind, CVector3 velocity, CVector3 pos,
                           void *region, f32 arg);
extern void func_00246690(void *actor, s32 event, CVector3 pos, s32 arg);

extern u8 D_800F7078[];
extern u8 D_80107590[];

/* Spawn an object of the given kind at pos, thrown upwards, and send it event 0x11D. */
void func_00406FE4(s32 kind, s32 count, CVector3 pos) {
    CVector3 velocity;
    void *region;
    void *spawned;

    region = func_00220A1C(D_800F7078, &pos);
    velocity.x = 0.0f;
    velocity.y = 512.0f;
    velocity.z = 0.0f;
    spawned = func_00227240(D_80107590, NULL, kind, velocity, pos, region, count);
    if (spawned != NULL) {
        func_00246690(spawned, 0x11D, pos, 0);
    }
}
