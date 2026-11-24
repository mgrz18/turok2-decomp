#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"

s32 Pak_GetNoteGameName(s32 bank, s32 slot, char **out) {
    if (gPakState[bank] != 3) {
        return -2;
    }
    *out = gPakDirectories[bank].notes[slot].game_name;
    return 0;
}
