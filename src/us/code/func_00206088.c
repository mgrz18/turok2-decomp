#include "common.h"
#include "m2c_macros.h"
#include "globals/cache.h"


s32 CCache__GetFreeCacheEntryCount(void) {
    return gCache.cacheFreeList.m_nCount;
}
