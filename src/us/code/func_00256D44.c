#include "common.h"

/* The player, as read here (the state at 0xA08 as in func_0024E9D0.c). */
typedef struct {
    /* 0x000 */ u8 pad0[0xA08];
    /* 0xA08 */ s16 state;
    /* 0xA0A */ u8 padA0A[0xC04 - 0xA0A];
    /* 0xC04 */ s32 unkC04;
} Player;

/* Whether the player is in state 3 with 0xC04 in 0x7DF..0x7E3. */
s32 func_00256D44(Player *arg0) {
    s32 temp_a0;
    s32 var_a1;

    var_a1 = 0;
    if (arg0->state == 3) {
        temp_a0 = arg0->unkC04;
        if (temp_a0 >= 0x7DF) {
            var_a1 = temp_a0 < 0x7E4;
        }
    }
    return var_a1;
}
