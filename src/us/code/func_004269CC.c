#include "common.h"
#include "structs/pak.h"

s32 func_004269CC(s32 bank, s32 slot, u8 **out) {
    if (D_80130C60[bank] != 3) {
        return -2;
    }
    *out = (u8 *)D_800C2224[bank].notes[slot].ext_name;
    return 0;
}
