#include "common.h"

/* The player, as func_0024FE10 sees it. */
typedef struct {
    /* 0x000 */ u8 pad0[0xA78];
    /* 0xA78 */ f32 speed;
} Player;

s32 func_0024E5F0(void *, s32, s32);    /* extern */
s32 func_00251A2C();                            /* extern */

void func_002563FC(Player *arg0, s32 arg1) {
    func_00251A2C();
    if (arg0->speed != 0.0f) {
        func_0024E5F0(arg0, arg1, 0x22);
    }
}
