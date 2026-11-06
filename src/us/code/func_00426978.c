#include "common.h"
#include "structs/pak.h"

s32 Pak_GetNoteGameName(s32 bank, s32 slot, char **out) {
    if (D_80130C60[bank] != 3) {
        return -2;
    }
    *out = D_800C2224[bank].notes[slot].game_name;
    return 0;
}
