/* WRONG CUT: real function 0x0041D6D4, size 0x98 (through 0x0041D76C): splat cut it in two
 * (func_0041D6D4, func_0041D740) between a lui and its store. */
#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"

extern s32 func_0025340C(void *players, void *player);

/* Name entry: delete the last character. */
s32 func_0041D6D4(s32 menu, Menu *c) {
    s32 i = func_0025340C(&D_80119870.m_PlayerList, c->player);

    D_800C1FB0[i].timer = 0;
    if (D_800C1FB0[i].length == 0) {
        return 0;
    }
    D_800C1FB0[i].length--;
    D_800C1FB0[i].cursor--;
    D_800C1FB0[i].name[D_800C1FB0[i].cursor] = 0;
    return 0;
}
