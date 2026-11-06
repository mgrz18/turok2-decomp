#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"

/* Reset the current player state and open a menu page on the player's controller. */

extern u8 D_0043B5EC[];

void CPlayerPool__GetIndex(void *state, void *player);
void func_00421170(void);
void MenuPool_OpenMenu(void *menu, void *page, void *player, void *controller, s32 flags);

s32 StatsMenu_SaveProfile_2(s32 arg0, Menu *event) {
    u8 *player = (u8 *) event->player;

    CPlayerPool__GetIndex(&D_80119870.m_PlayerList, player);
    func_00421170();
    MenuPool_OpenMenu(D_80119870.m_MenuPool, D_0043B5EC, player, *(void **)(player + 0xA50), 0);
    return 1;
}
