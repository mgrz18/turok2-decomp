#include "common.h"

s32 DList_AddTail(s32, s32);                    /* extern */
s32 DList_Remove(void *, s32);                 /* extern */

typedef struct {
    /* 0x0000 */ u8 pad0[0x6A88];
    /* 0x6A88 */ s32 pending;
} Unk00235FF4;

s32 Swoosh_TakeNode(Unk00235FF4 *arg0, s32 arg1) {
    s32 temp_s0;

    temp_s0 = arg0->pending;
    if (temp_s0 != 0) {
        DList_Remove(&arg0->pending, temp_s0);
        DList_AddTail(arg1 + 0x40, temp_s0);
    }
    return temp_s0;
}
