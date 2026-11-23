/* WRONG CUT: real function 0x0041A3B0, size 0x24: splat started it 4 bytes early at a
 * stray word (0x41A3AC, a `bc1f` encoding never executed; func_0041A3A8 is the nop
 * padding before it). */
#include "common.h"
#include "structs/menu.h"

extern void CPlayer__StartGame(void *player);

s32 ContinueMenu_Yes(s32 menu, Menu *c) {
    CPlayer__StartGame(c->player);
    return 1;
}

#include "structs/player.h"
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
