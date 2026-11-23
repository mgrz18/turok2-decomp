#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } Vec3i;
extern f32 CVector3__Normalize(void *);

void CSun__SetSunDir(void *arg0, Vec3i *arg1) {
    *(Vec3i *)((s8 *)arg0 + 0x14) = *arg1;
    CVector3__Normalize((s8 *)arg0 + 0x14);
}
