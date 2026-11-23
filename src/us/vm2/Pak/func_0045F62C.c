#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"

s32 Pak_CountEmptyNotes_2(s32 i, s32 *freeCount) {
    s32 j;

    if (gPakState[i] != 3) {
        return -2;
    }
    *freeCount = 0;
    if (gPakError[i] == 0) {
        for (j = 0; j < 16; j++) {
            if (gPakDirectories[i].notes[j].file_size == 0) {
                (*freeCount)++;
            }
        }
    }
    return gPakError[i];
}
