#include "common.h"
#include "m2c_macros.h"

extern s32 func_00259E60(void *, s32, void *);

s32 func_0025EFF8(void *arg0, s32 arg1) {
    u16 kind;
    void *sub;

    kind = M2C_FIELD(arg0, u16 *, 0xB8);
    sub = M2C_FIELD(arg0, s32 *, 0x14) + 0x14;
    if (kind == 0x1F9 || kind == 0x385) {
        switch (M2C_FIELD(sub, s32 *, 8)) {
            case 0x2B16:
            case 0x2B20:
            case 0x2B2A:
            case 0x2B34:
            case 0x2B3E:
                break;
            default:
                return func_00259E60(arg0, arg1, sub);
        }
    }
    return M2C_FIELD(sub, s32 *, 8);
}
