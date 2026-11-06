#include "common.h"
#include "structs/pak.h"

s32 Pak_GetNoteGameName_2(s32 bank, s32 slot, char **out) {
    if (gPakState[bank] != 3) {
        return -2;
    }
    *out = gPakDirectories[bank].notes[slot].game_name;
    return 0;
}
