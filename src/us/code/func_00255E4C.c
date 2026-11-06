#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

/* Menu state: the current player pointer is the word at 0x20 (D_80119890). */
extern char D_800C1E54[];

extern void MenuPool_OpenMenu(void *a, char *text, CPlayer *player, s32 d, void *e);

void PlayerState17_Enter(CPlayer *player) {
    CPlayer *current = D_80119870.m_PlayerList.m_ActiveList.m_pHead;

    P_S32(player, 0x1298) = 0;
    P_S32(player, 0xD4) &= ~0x800000;
    if (player == current) {
        MenuPool_OpenMenu(D_80119870.m_MenuPool, D_800C1E54, player, P_S32(player, 0xA50), NULL);
    }
}
