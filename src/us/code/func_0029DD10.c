#include "common.h"

s32 osCreateMesgQueue(s32 *, s32 *, s32); /* extern */
s32 osSendMesg(s32 *, s32, s32); /* extern */
extern s32 D_800B8990;
extern s32 D_80124DB0;
extern s32 D_80124DB8;

void __osPiCreateAccessQueue(void) {
    D_800B8990 = 1;
    osCreateMesgQueue(&D_80124DB8, &D_80124DB0, 1);
    osSendMesg(&D_80124DB8, 0, 0);
}
