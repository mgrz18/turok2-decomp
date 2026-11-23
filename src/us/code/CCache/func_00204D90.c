#include "common.h"
#include "structs/cache.h"

s32 CMemEntry__IsLocked(CMemEntry *arg0) {
    return arg0->flags & 0x100;
}
