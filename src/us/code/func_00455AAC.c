#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"

/* Menu text item: print one u16 stat of the current player's record (64-byte
 * records, field at 0x8011AB5C) at the end of the item's text. */

extern u16 D_8011AB5C[];
extern char D_800C1330[];

s32 CPlayerPool__GetIndex(void *state, void *player);
s32 MenuItem_TextLength(MenuItem *item);
void sprintf(char *dst, char *fmt, s32 value);

s32 StatsMenu_FragsText_2(MenuItem *item, Menu *event) {
    s32 value = D_8011AB5C[CPlayerPool__GetIndex(&D_80119870.m_PlayerList, event->player) * 32];

    sprintf(item->text + (MenuItem_TextLength(item) - 4), D_800C1330, value);
    return 0;
}
