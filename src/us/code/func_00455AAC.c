#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"

/* Menu text item: print one u16 stat of the current player's record (64-byte
 * records, field at 0x8011AB5C) at the end of the item's text. */

extern u16 D_8011AB5C[];
extern char D_800C1330[];

s32 func_0025340C(void *state, void *player);
s32 func_004160F0(MenuItem *item);
void func_0029E3E0(char *dst, char *fmt, s32 value);

s32 func_00455AAC(MenuItem *item, Menu *event) {
    s32 value = D_8011AB5C[func_0025340C(&D_80119870.m_PlayerList, event->player) * 32];

    func_0029E3E0(item->text + (func_004160F0(item) - 4), D_800C1330, value);
    return 0;
}
