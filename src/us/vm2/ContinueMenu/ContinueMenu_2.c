#include "common.h"
#include "structs/menu.h"

void CPlayer__StartGame_2(void *player);

s32 ContinueMenu_Yes_2(void *item, Menu *menu) {
    CPlayer__StartGame_2(menu->player);
    return 1;
}

#include "structs/player.h"
#include "structs/game.h"

extern s32 gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern void CEngineApp__QuitToLegal_2(void *a);

s32 ContinueMenu_No_2(s32 a, Menu *b) {
    if (gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount == 1) CEngineApp__QuitToLegal_2(&gEngineApp); else b->player->m_bActive = 0;
    return 1;
}
