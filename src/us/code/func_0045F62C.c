#include "common.h"
#include "structs/pak.h"

s32 func_0045F62C(s32 i, s32 *freeCount) {
    s32 j;

    if (D_80130C60[i] != 3) {
        return -2;
    }
    *freeCount = 0;
    if (D_80130C70[i] == 0) {
        for (j = 0; j < 16; j++) {
            if (D_800C2224[i].notes[j].file_size == 0) {
                (*freeCount)++;
            }
        }
    }
    return D_80130C70[i];
}
