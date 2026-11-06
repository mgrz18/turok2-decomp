#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "structs/player_options.h"

/* Menu text item: print the current player's suicide count (his
 * CPlayerOptions in D_8011AB58) at the end of the item's text. */

extern CPlayerOptions D_8011AB58[];
extern char D_800C1330[];

s32 CPlayerPool__GetIndex(void *state, void *player);
s32 MenuItem_TextLength_2(MenuItem *item);
void sprintf(char *dst, char *fmt, s32 value);

s32 StatsMenu_SuicidesText_2(MenuItem *item, Menu *event) {
    s32 value = D_8011AB58[CPlayerPool__GetIndex(&D_80119870.m_PlayerList, event->player)].m_Suicides;

    sprintf(item->text + (MenuItem_TextLength_2(item) - 4), D_800C1330, value);
    return 0;
}
