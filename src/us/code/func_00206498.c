#include "common.h"

s32 CList__AddTail(s32, s32);
s32 func_002005D0(s32, s32, s32);

/* A queued request on the D_800D81F4 list (protected by the D_800D8DB0
 * message queue): sorted by key, with a busy count and state flags. */
typedef struct CRequest {
    /* 0x00 */ u32 key;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 count;
    /* 0x0C */ s32 flags;
    /* 0x10 */ u8 pad10[0x24 - 0x10];
    /* 0x24 */ struct CRequest *next;
} CRequest;

extern CRequest *D_800D81F4;

void func_00206498(s32 arg0, CRequest *arg1) {
    CRequest *var_s0;

    var_s0 = D_800D81F4;
    if (var_s0 != NULL) {
loop_1:
        if (var_s0->key > arg1->key) {
            func_002005D0((s32) &D_800D81F4, (s32) var_s0, (s32) arg1);
        } else {
            var_s0 = var_s0->next;
            if (var_s0 != NULL) {
                goto loop_1;
            }
        }
        if (var_s0 == NULL) {
            goto block_5;
        }
    } else {
block_5:
        CList__AddTail((s32) &D_800D81F4, (s32) arg1);
    }
    arg1->flags = arg1->flags | 0x1000;
}
