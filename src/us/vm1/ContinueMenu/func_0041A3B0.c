/* WRONG CUT: real function 0x0041A3B0, size 0x24: splat started it 4 bytes early at a
 * stray word (0x41A3AC, a `bc1f` encoding never executed; func_0041A3A8 is the nop
 * padding before it). */
#include "common.h"
#include "structs/menu.h"

extern void CPlayer__StartGame(void *player);

s32 ContinueMenu_Yes(s32 menu, Menu *c) {
    CPlayer__StartGame(c->player);
    return 1;
}
