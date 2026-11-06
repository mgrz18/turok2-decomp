#include "common.h"
#include "m2c_macros.h"

extern s32 gCache_freeEntryCount;

s32 CCache__GetFreeMemEntryCount(void) {
    return gCache_freeEntryCount;
}
