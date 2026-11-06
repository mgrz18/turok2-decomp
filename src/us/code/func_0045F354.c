#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"

s32 CCache__Deallocate(s32, s32);                /* extern */
s32 CCache__Flush(s32);                     /* extern */

void Pak_FreeDirectory_2(void) {
    CCache__Flush(0);
    if (gPakStateBuffer != 0) {
        CCache__Deallocate(0, gPakStateBuffer);
        gPakStateBuffer = 0;
        gPakDirectories = 0;
    }
    gPakDirectoryAllocated = 0;
}
