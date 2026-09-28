#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } Vec3i;
extern f32 func_0020F040(void *);

void func_00228BC4(void *arg0, Vec3i *arg1) {
    *(Vec3i *)((s8 *)arg0 + 0x14) = *arg1;
    func_0020F040((s8 *)arg0 + 0x14);
}
