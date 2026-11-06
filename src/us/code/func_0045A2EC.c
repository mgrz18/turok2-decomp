#include "common.h"
#include "structs/save.h"
#include "globals/pak.h"

s32 CCache__Deallocate(s32, s32);                /* extern */
s32 CCache__Flush(s32);                     /* extern */

void Pak_FreeSaveBuffers_2(void) {
    if ((gSaveBufferHandle != 0) || (gLoadBufferHandle != 0)) {
        CCache__Flush(0);
    }
    if (gSaveBufferHandle != 0) {
        CCache__Deallocate(0, gSaveBufferHandle);
    }
    if (gLoadBufferHandle != 0) {
        CCache__Deallocate(0, gLoadBufferHandle);
    }
    gSaveBufferHandle = 0;
    gLoadBufferHandle = 0;
    gSaveBuffer = 0;
    gLoadBuffer = 0;
}
