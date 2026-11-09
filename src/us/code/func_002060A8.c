#include "common.h"
#include "m2c_macros.h"
#include "globals/cache.h"
#include "protos/cache.h"


s32 CCache__GetMemEntryCount(void) {
    return gCache.memAgeList.m_nCount;
}
