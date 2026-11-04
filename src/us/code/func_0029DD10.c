#include "common.h"

s32 func_0029B030(s32 *, s32 *, s32); /* extern */
s32 osSendMesg(s32 *, s32, s32); /* extern */
extern s32 D_800B8990;
extern s32 D_80124DB0;
extern s32 D_80124DB8;

void func_0029DD10(void) {
    D_800B8990 = 1;
    func_0029B030(&D_80124DB8, &D_80124DB0, 1);
    osSendMesg(&D_80124DB8, 0, 0);
}
