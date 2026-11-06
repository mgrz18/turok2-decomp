#include "common.h"
#include "m2c_macros.h"

extern s32 gPakDirectoryAllocated;

s32 Pak_IsDirectoryAllocated(void) {
    return gPakDirectoryAllocated;
}
