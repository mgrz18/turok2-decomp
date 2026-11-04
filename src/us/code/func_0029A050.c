#include "common.h"

s32 osSendMesg(s32 *, s32, s32); /* extern */
extern s32 D_80121678;

void func_0029A050(void) {
    osSendMesg(&D_80121678, 0, 0);
}
