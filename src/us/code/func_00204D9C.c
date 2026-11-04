#include "common.h"
#include "structs/cache.h"

void func_00204D9C(CMemEntry *arg0) {
    arg0->lockCount = (s32) (arg0->lockCount + 1);
    arg0->flags = (s32) (arg0->flags | 0x100);
}
