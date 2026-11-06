#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/camera.h"

extern char D_0043ABF0;
extern char D_0043AC14;
extern char D_0043AC38;
extern char D_0043AC5C;

void CPlayer__StopLoopingSounds(CPlayer *);
s32 CPlayerPool__GetIndex(CPlayerPool *players, CPlayer *player);
void MenuPool_OpenMenu(void *pool, void *screen, CPlayer *p, void *controller, s32);

/* CPlayer__StopLoopingSounds takes the player (as in CPlayer__Kill): the call keeps it in $a0,
 * which is what lets sched2 keep the mask's lui/ori together. */
/* Clears the player's flag 0x800000 and, in multiplayer, opens the screen of
 * the player's slot (one of four) on the player's camera. */
void PlayerState16_Enter(CPlayer *player) {
    char *screen;

    player->m_ShockedTimer = 0;
    player->m_CharacterActor.m_dwFlags &= ~0x800000;
    CPlayer__StopLoopingSounds(player);
    if (D_80119870.m_Settings.multiplayer) {
        switch ((u32)CPlayerPool__GetIndex(&D_80119870.m_PlayerList, player)) {
        case 0:
        default:
            screen = &D_0043ABF0;
            break;
        case 1:
            screen = &D_0043AC14;
            break;
        case 2:
            screen = &D_0043AC38;
            break;
        case 3:
            screen = &D_0043AC5C;
            break;
        }
        MenuPool_OpenMenu(player->m_pCamera->m_MenuPool, screen, player, player->m_ContState.m_pContState, 0);
    }
}
