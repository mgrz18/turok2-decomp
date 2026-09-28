#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } V3;
typedef struct { f32 x, y, z; } V3f;

extern u8 D_800B2C50[];
extern void func_0027AD00(void *, void *);

void func_0027AD24(u8 *arg0, s32 arg1, s32 arg2) {
    M2C_FIELD(arg0, s32 *, 0x40) = arg1;
    M2C_FIELD(arg0, s32 *, 0x3C) = arg2;
    M2C_FIELD(arg0, s32 *, 0x44) = 100;
    func_0027AD00(arg0, D_800B2C50);
}
