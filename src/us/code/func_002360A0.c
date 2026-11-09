#include "common.h"

s32 CMtxF__ClampEntries(s32);
s32 Swoosh_NewNode(s32, s32, s32);

s32 CMtxF__Copy(s32 *, s32);              /* extern */
s32 CMtxF__ScaleAxes(s32 *, f32, f32, f32);    /* extern */

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
                CMtxF__Copy(sp10, arg2 + (var_s0->ref->index << 6));
                CMtxF__ScaleAxes(sp10, 0.00000015258789f, 0.00000015258789f, 0.00000015258789f);
                CMtxF__ClampEntries((sptr) sp10);
                Swoosh_NewNode((sptr) arg0, (sptr) var_s0, (sptr) sp10);
            }
            var_s0 = var_s0->next;
        } while (var_s0 != NULL);
    }
}
