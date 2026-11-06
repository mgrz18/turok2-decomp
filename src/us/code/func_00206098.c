#include "common.h"
#include "m2c_macros.h"
#include "globals/cache.h"


s32 CCache__GetFreeMemEntryCount(void) {
    return gCache_freeEntryCount;
}
