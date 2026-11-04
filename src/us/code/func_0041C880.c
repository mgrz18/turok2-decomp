/* WRONG CUT: real function 0x0041C880, size 0x4C (through 0x0041C8CC): splat cut it in two
 * (func_0041C880, func_0041C89C) in the middle of a call's argument setup. */
#include "common.h"
#include "structs/game.h"

extern s32 func_0041648C(void *c, s32 value, s32 step, s32 min, s32 max, s32 wrap);

s32 func_0041C880(s32 menu, void *c) {
    GameSettings *s = &D_8011AAD8;

    s->unkC = func_0041648C(c, s->unkC, 0x10, 0, 0x80, 0);
    return 0;
}
