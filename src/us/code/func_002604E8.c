#include "common.h"
#include "m2c_macros.h"

extern s32 func_00220260(s32, s32, s32);
extern void func_0026E110(void *, s32, void *);
extern u8 D_800F7078[];

void func_002604E8(void *arg0) {
    void *sub = M2C_FIELD(arg0, void **, 0x14);

    if (!(M2C_FIELD(arg0, s32 *, 0xD4) & 0x80000)) {
        if (!(M2C_FIELD(sub, s32 *, 0x14) & 0x20)) {
            func_00220260((s32) D_800F7078, (s32) arg0, 1);
        }
        func_0026E110(arg0, 1, arg0);
    }
}
