#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"
#include "protos/cache.h"


s32 memset(s32, s32, s32);       /* extern */

void Pak_AllocDirectory_2(void) {
    s32 *temp_v0;
    sptr temp_a0;

    temp_v0 = CCache__Allocate(0, 0x810, 0x23, (sptr) "Mempak pakstate");
    temp_a0 = *temp_v0;
    gPakStateBuffer = (CMemEntry *)temp_v0;
    gPakDirectories = (PakDirectory *)temp_a0;
    memset(temp_a0, 0, 0x810);
    gPakDirectoryAllocated = 1;
}
