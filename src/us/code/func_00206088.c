#include "common.h"
#include "m2c_macros.h"

extern s32 gCache_cacheFreeList_m_nCount;

s32 CCache__GetFreeCacheEntryCount(void) {
    return gCache_cacheFreeList_m_nCount;
}
