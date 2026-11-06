#include "common.h"

s32 osRecvMesg(void *, s32, s32);         /* extern */
extern u8 gPakAccessQueue[];

s32 Controller_Lock(s32 arg0) {
    return osRecvMesg(gPakAccessQueue, 0, arg0) == 0;
}
