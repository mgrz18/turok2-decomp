#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/menu_desc.h"
#include "protos/player.h"

/* Reset the current player state and open a menu page on the player's controller. */

void func_0045A170(void);
void MenuPool_OpenMenu_2(void *menu, void *page, void *player, void *controller, s32 flags);

s32 StatsMenu_SaveProfile_2(s32 arg0, Menu *event) {
    u8 *player = (u8 *) event->player;

    CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, player);
    func_0045A170();
    MenuPool_OpenMenu_2(gEngineApp_m_Game.m_MenuPool, &gPakSaveProfileMenuDesc_2, player, *(void **)(player + 0xA50), 0);
    return 1;
}
