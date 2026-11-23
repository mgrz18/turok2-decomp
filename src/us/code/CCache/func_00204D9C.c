#include "common.h"
#include "structs/cache.h"

void CMemEntry__Lock(CMemEntry *arg0) {
    arg0->lockCount = (s32) (arg0->lockCount + 1);
    arg0->flags = (s32) (arg0->flags | 0x100);
}
