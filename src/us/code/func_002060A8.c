#include "common.h"
#include "m2c_macros.h"

extern s32 gCache_memAgeList_m_nCount;

s32 CCache__GetMemEntryCount(void) {
    return gCache_memAgeList_m_nCount;
}
