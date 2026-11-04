#include "common.h"
#include "structs/vector.h"

extern u8 D_80107590[];
u8 *func_00227240(void *table, void *slot, s32 arg, CVector3 origin, CVector3 pos, s32 b, s32 c);
void func_00246690(void *actor, s32 event, CVector3 pos, s32 n);

void func_002637F4(u8 *actor, u8 *state, CVector3 pos, s32 arg, s32 kind) {
    u8 *spawned;

    spawned = func_00227240(D_80107590, state + 0x10C, kind, *(CVector3 *) (actor + 0x18), pos, arg, 0);
    if (spawned != NULL) {
        *(s32 *) (state + 0x110) -= 1;
        func_00246690(spawned, 0x11D, *(CVector3 *) (spawned + 0x4), 0);
    }
}
