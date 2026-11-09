#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/player.h"

/* Menu text item: print one u16 stat of the current player's record (64-byte
 * records, field at 0x8011AB5A) at the end of the item's text. */

extern u16 D_8011AB5A[];
extern char D_800C1330[];

s32 MenuItem_TextLength_2(MenuItem *item);
#ifdef TARGET_PORT
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
void sprintf(char *dst, const char *fmt, ...);
#else
void sprintf(char *dst, char *fmt, s32 value);
#endif

s32 StatsMenu_DeathsText_2(MenuItem *item, Menu *event) {
    s32 value = D_8011AB5A[CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, event->player) * 32];

    sprintf(item->text + (MenuItem_TextLength_2(item) - 4), D_800C1330, value);
    return 0;
}
