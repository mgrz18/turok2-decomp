#include "common.h"
#include "structs/save.h"
#include "globals/pak.h"

extern void CCache__Deallocate(sptr, void *);
extern void CCache__Flush_Arg(s32) __asm__("CCache__Flush"); /* the calls pass 0; the definition (code/CCache) takes none */

void Pak_FreeSaveBuffers_2(void) {
    if ((gSaveBufferHandle != 0) || (gLoadBufferHandle != 0)) {
        CCache__Flush_Arg(0);
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
