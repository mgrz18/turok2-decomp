#include "common.h"

s32 osRecvMesg(void *, s32, s32);         /* extern */
extern u8 D_800F6758[];

s32 func_00285878(s32 arg0) {
    return osRecvMesg(D_800F6758, 0, arg0) == 0;
}
