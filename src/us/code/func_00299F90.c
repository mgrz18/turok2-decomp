#include "common.h"

void osCreateMesgQueue(void *, void *, s32); /* extern */
void osSendMesg(void *, s32, s32); /* extern */
extern s32 D_800B7A50;
extern u8 D_80121670[];
extern u8 D_80121678[];

void __osSiCreateAccessQueue(void) {
    D_800B7A50 = 1;
    osCreateMesgQueue(D_80121678, D_80121670, 1);
    osSendMesg(D_80121678, 0, 0);
}
