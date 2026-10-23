#include "common.h"

typedef struct { f32 x, y, z; } Point3;

extern u8 D_80107590[];
u8 *func_00227240(void *table, void *slot, s32 arg, Point3 origin, Point3 pos, s32 b, s32 c);
void func_00246690(void *actor, s32 event, Point3 pos, s32 n);

void func_002637F4(u8 *actor, u8 *state, Point3 pos, s32 arg, s32 kind) {
    u8 *spawned;

    spawned = func_00227240(D_80107590, state + 0x10C, kind, *(Point3 *) (actor + 0x18), pos, arg, 0);
    if (spawned != NULL) {
        *(s32 *) (state + 0x110) -= 1;
        func_00246690(spawned, 0x11D, *(Point3 *) (spawned + 0x4), 0);
    }
}
