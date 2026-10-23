#include "common.h"

extern s32 D_80130990;
extern s32 D_800C2028;

s32 func_00421690(void *arg0) {
    if (D_80130990 != 0) {
        return D_800C2028;
    }
    return *(s8 *)(*(u8 **)((u8 *)arg0 + 0x20) + 4);
}
