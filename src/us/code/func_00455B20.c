#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/player.h"

/* Menu text item: print one u16 stat of the current player's record (64-byte
 * records, field at 0x8011AB5A) at the end of the item's text. */


s32 MenuItem_TextLength_2(MenuItem *item);
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
void sprintf(char *dst, const char *fmt, ...);

s32 StatsMenu_DeathsText_2(MenuItem *item, Menu *event) {
    s32 value = gEngineApp_m_Settings.slots[CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, event->player)].m_Deaths;

    sprintf(item->text + (MenuItem_TextLength_2(item) - 4), sStatsMenuFmt_04d, value);
    return 0;
}
