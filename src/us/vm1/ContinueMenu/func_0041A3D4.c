#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/game.h"

extern s32 gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern void CEngineApp__QuitToLegal(char *);

s32 ContinueMenu_No(s32 menu, Menu *c) {
    if (gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount == 1) {
        CEngineApp__QuitToLegal(&gEngineApp);
    } else {
        c->player->m_bActive = 0;
    }
    return 1;
}
