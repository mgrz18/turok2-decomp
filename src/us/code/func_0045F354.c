#include "common.h"
#include "structs/pak.h"

s32 CCache__Deallocate(s32, s32);                /* extern */
s32 CCache__Flush(s32);                     /* extern */
extern s32 D_800C2220;
extern s32 D_800C2228;

void Pak_FreeDirectory_2(void) {
    CCache__Flush(0);
    if (D_800C2228 != 0) {
        CCache__Deallocate(0, D_800C2228);
        D_800C2228 = 0;
        D_800C2224 = 0;
    }
    D_800C2220 = 0;
}
