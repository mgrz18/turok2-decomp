#include "common.h"
#include "structs/pak.h"

s32 Pak_GetNoteExtName_2(s32 i, s32 j, char **out) {
    if (D_80130C60[i] != 3) {
        return -2;
    }
    *out = D_800C2224[i].notes[j].ext_name;
    return 0;
}
