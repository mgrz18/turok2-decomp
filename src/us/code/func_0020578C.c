#include "common.h"
#include "globals/cache.h"
#include "protos/cache.h"

extern void osSendMesg(void *, s32, s32);

void CCache__PostDisplayListDone(s32 unused, s32 x) {
    osSendMesg(&gCache_queue, x, 1);
}
