#include "common.h"
#include "structs/pak.h"

s32 Pak_GetNoteExtName(s32 bank, s32 slot, u8 **out) {
    if (gPakState[bank] != 3) {
        return -2;
    }
    *out = (u8 *)gPakDirectories[bank].notes[slot].ext_name;
    return 0;
}
