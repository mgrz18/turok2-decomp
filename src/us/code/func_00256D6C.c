#include "common.h"

/* The player, as read here (the state at 0xA08 as in func_0024E9D0.c). */
typedef struct {
    /* 0x000 */ u8 pad0[0xA08];
    /* 0xA08 */ s16 state;
} Player;

s32 func_00256D6C(Player *arg0) {
    return arg0->state == 0xE;
}
