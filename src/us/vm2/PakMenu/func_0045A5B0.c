#include "common.h"
#include "globals/pak.h"


s32 PakMenu_GetPort_2(void *arg0) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return *(s8 *)(*(u8 **)((u8 *)arg0 + 0x20) + 4);
}
