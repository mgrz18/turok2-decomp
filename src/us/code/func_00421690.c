#include "common.h"

extern s32 D_80130990;
extern s32 D_800C2028;

s32 PakMenu_GetPort(void *arg0) {
    if (D_80130990 != 0) {
        return D_800C2028;
    }
    return *(s8 *)(*(u8 **)((u8 *)arg0 + 0x20) + 4);
}
