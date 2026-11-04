#include "common.h"
#include "structs/vector.h"

extern u8 D_800F8928[];
s32 func_0022F350(void *table, void *actor, void *parent, void *slot, s32 a, s32 kind, CVector3 origin,
                  CQuaternion rot, CVector3 pos, s32 arg, s32 b, s32 c);

void func_00263954(u8 *actor, u8 *state, CVector3 pos, s32 arg, s32 kind) {
    *(s32 *) (state + 0x110) -= func_0022F350(D_800F8928, actor, actor, state + 0x10C, 0, kind,
                                              *(CVector3 *) (actor + 0x18), *(CQuaternion *) (actor + 0x40), pos,
                                              arg, -1, 0);
}
