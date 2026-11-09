#include "common.h"
#include "globals/cache.h"
#include "protos/cache.h"
#include "protos/ultra.h"


void CCache__PostDisplayListDone(s32 unused, s32 x) {
    osSendMesg(&gCache_queue, x, 1);
}
