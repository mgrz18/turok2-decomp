#include "common.h"

/* The player (func_002532A8(&D_80119870)); include/structs/player.h does not
 * line up here (its m_Mode is at 0xA0C, the code has the state at 0xA08), so
 * only the flag set here is named. */
typedef struct {
    /* 0x000 */ u8 pad0[0x9F0];
    /* 0x9F0 */ s32 unk9F0; /* set once func_00284204 holds; also read by func_00289A9C */
} Player;

s32 func_00248BA8(s32, s32, s32, s32);

void *func_002532A8(s32 *);                     /* extern */
s32 func_00284204();                                /* extern */
extern s32 D_80119870;

void func_00440D6C(void) {
    Player *temp_v0;

    if (func_00284204() != 0) {
        temp_v0 = func_002532A8(&D_80119870);
        if (temp_v0 != NULL) {
            temp_v0->unk9F0 = 1;
            func_00248BA8((s32) temp_v0, 0x1F40, 2, 0);
        }
    }
}
