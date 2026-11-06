#include "common.h"

s32 osSendMesg(s32 *, s32, s32); /* extern */
extern s32 D_800F6758;

void Controller_Unlock(void) {
    osSendMesg(&D_800F6758, 0, 0);
}
