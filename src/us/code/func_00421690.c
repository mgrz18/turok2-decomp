#include "common.h"
#include "globals/pak.h"

extern s32 gPakMenuAnyPak;

s32 PakMenu_GetPort(void *arg0) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return *(s8 *)(*(u8 **)((u8 *)arg0 + 0x20) + 4);
}
