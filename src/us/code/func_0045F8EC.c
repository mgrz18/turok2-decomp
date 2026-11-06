#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"

s32 Pak_GetNoteExtName_2(s32 i, s32 j, char **out) {
    if (gPakState[i] != 3) {
        return -2;
    }
    *out = gPakDirectories[i].notes[j].ext_name;
    return 0;
}
