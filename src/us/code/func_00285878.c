#include "common.h"
#include "globals/pak.h"
#include "protos/ultra.h"


s32 Controller_Lock(s32 arg0) {
    return osRecvMesg(&gPakAccessQueue, 0, arg0) == 0;
}
