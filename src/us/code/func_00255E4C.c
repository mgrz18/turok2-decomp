#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/menu.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

/* Menu state: the current player pointer is the word at 0x20 (gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead). */
extern char D_800C1E54[];


void PlayerState17_Enter(CPlayer *player) {
    CPlayer *current = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;

    P_S32(player, 0x1298) = 0;
    P_S32(player, 0xD4) &= ~0x800000;
    if (player == current) {
        MenuPool_OpenMenu(gEngineApp_m_Game.m_MenuPool, D_800C1E54, player, P_S32(player, 0xA50), NULL);
    }
}
