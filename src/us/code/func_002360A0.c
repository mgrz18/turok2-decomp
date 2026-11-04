#include "common.h"

s32 func_0020F7EC(s32);
s32 func_002333C0(s32, s32, s32);

s32 func_0020FF80(s32 *, s32);              /* extern */
s32 func_0021034C(s32 *, f32, f32, f32);    /* extern */

typedef struct {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ s32 index;
} Ref002360A0;

typedef struct Node002360A0 {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ struct Node002360A0 *next;
    /* 0x08 */ Ref002360A0 *ref;
    /* 0x0C */ u8 padC[0x1C - 0xC];
    /* 0x1C */ s32 inst;
    /* 0x20 */ u8 pad20[0x24 - 0x20];
    /* 0x24 */ f32 strength;
} Node002360A0;

typedef struct {
    /* 0x0000 */ u8 pad0[0x7520];
    /* 0x7520 */ Node002360A0 *head;
} World002360A0;

void func_002360A0(World002360A0 *arg0, s32 arg1, s32 arg2) {
    u8 sp10[0x40];
    Node002360A0 *var_s0;

    var_s0 = arg0->head;
    if (var_s0 != NULL) {
        do {
            if ((var_s0->inst == arg1) && (var_s0->strength > 0.0f)) {
                func_0020FF80(sp10, arg2 + (var_s0->ref->index << 6));
                func_0021034C(sp10, 0.00000015258789f, 0.00000015258789f, 0.00000015258789f);
                func_0020F7EC((s32) sp10);
                func_002333C0((s32) arg0, (s32) var_s0, (s32) sp10);
            }
            var_s0 = var_s0->next;
        } while (var_s0 != NULL);
    }
}
