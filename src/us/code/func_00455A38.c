#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/player.h"

/* Menu text item: print one u16 stat of the current player's record (64-byte
 * records, field at 0x8011AB5E) at the end of the item's text. */

extern u16 D_8011AB5E[];
extern char D_800C1330[];

s32 MenuItem_TextLength_2(MenuItem *item);
void sprintf(char *dst, char *fmt, s32 value);

s32 StatsMenu_TagsText_2(MenuItem *item, Menu *event) {
    s32 value = D_8011AB5E[CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, event->player) * 32];

    sprintf(item->text + (MenuItem_TextLength_2(item) - 4), D_800C1330, value);
    return 0;
}
