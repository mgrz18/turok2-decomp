#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"

extern void CCache__Flush(s32);
extern void CCache__Deallocate(sptr, void **);

void Pak_FreeDirectory_2(void) {
    CCache__Flush(0);
    if (gPakStateBuffer != 0) {
        CCache__Deallocate(0, (void **) gPakStateBuffer);
        gPakStateBuffer = 0;
        gPakDirectories = 0;
    }
    gPakDirectoryAllocated = 0;
}
