#include "common.h"

s32 osSendMesg(s32 *, s32, s32); /* extern */
extern s32 D_80124DB8;

void func_0029DDD0(void) {
    osSendMesg(&D_80124DB8, 0, 0);
}
