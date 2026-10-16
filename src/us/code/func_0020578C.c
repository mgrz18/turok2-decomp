#include "common.h"

extern char D_800D8450[];
extern void osSendMesg(void *, s32, s32);

void func_0020578C(s32 unused, s32 x) {
    osSendMesg(D_800D8450, x, 1);
}
