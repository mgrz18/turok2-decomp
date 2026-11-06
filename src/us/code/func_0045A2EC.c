#include "common.h"
#include "structs/save.h"

s32 CCache__Deallocate(s32, s32);                /* extern */
s32 CCache__Flush(s32);                     /* extern */
extern s32 D_800C2010;
extern s32 D_800C2014;

void Pak_FreeSaveBuffers_2(void) {
    if ((D_800C2010 != 0) || (D_800C2014 != 0)) {
        CCache__Flush(0);
    }
    if (D_800C2010 != 0) {
        CCache__Deallocate(0, D_800C2010);
    }
    if (D_800C2014 != 0) {
        CCache__Deallocate(0, D_800C2014);
    }
    D_800C2010 = 0;
    D_800C2014 = 0;
    D_800C2018 = 0;
    D_800C201C = 0;
}
