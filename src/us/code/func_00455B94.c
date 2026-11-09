#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "structs/player_options.h"
#include "globals/game.h"
#include "protos/player.h"

/* Menu text item: print the current player's suicide count (his
 * CPlayerOptions in gEngineApp_m_Settings_slots) at the end of the item's text. */

extern char D_800C1330[];

s32 MenuItem_TextLength_2(MenuItem *item);
#ifdef TARGET_PORT
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
void sprintf(char *dst, const char *fmt, ...);
#else
void sprintf(char *dst, char *fmt, s32 value);
#endif

s32 StatsMenu_SuicidesText_2(MenuItem *item, Menu *event) {
    s32 value = gEngineApp_m_Settings.slots[CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, event->player)].m_Suicides;

    sprintf(item->text + (MenuItem_TextLength_2(item) - 4), D_800C1330, value);
    return 0;
}
