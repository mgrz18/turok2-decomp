#include "common.h"

typedef struct { f32 x, y, z; } Point3;
typedef struct { f32 x, y, z, w; } Quat4;

extern u8 D_800F8928[];
s32 func_0022F350(void *table, void *actor, void *parent, void *slot, s32 a, s32 kind, Point3 origin,
                  Quat4 rot, Point3 pos, s32 arg, s32 b, s32 c);

void func_00263954(u8 *actor, u8 *state, Point3 pos, s32 arg, s32 kind) {
    *(s32 *) (state + 0x110) -= func_0022F350(D_800F8928, actor, actor, state + 0x10C, 0, kind,
                                              *(Point3 *) (actor + 0x18), *(Quat4 *) (actor + 0x40), pos,
                                              arg, -1, 0);
}
