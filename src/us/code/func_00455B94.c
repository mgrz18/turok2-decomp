#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "structs/player_options.h"

/* Menu text item: print the current player's suicide count (his
 * CPlayerOptions in D_8011AB58) at the end of the item's text. */

extern CPlayerOptions D_8011AB58[];
extern char D_800C1330[];

s32 func_0025340C(void *state, void *player);
s32 func_004160F0(MenuItem *item);
void func_0029E3E0(char *dst, char *fmt, s32 value);

s32 func_00455B94(MenuItem *item, Menu *event) {
    s32 value = D_8011AB58[func_0025340C(&D_80119870.m_PlayerList, event->player)].m_Suicides;

    func_0029E3E0(item->text + (func_004160F0(item) - 4), D_800C1330, value);
    return 0;
}
