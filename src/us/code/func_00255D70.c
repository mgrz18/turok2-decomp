#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/camera.h"

extern char gStatsSaveDoneMenuDesc1;
extern char gStatsSaveDoneMenuDesc2;
extern char gStatsSaveDoneMenuDesc3;
extern char gStatsSaveDoneMenuDesc4;

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
    if (gEngineApp_m_Game.m_Settings.multiplayer) {
        switch ((u32)CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, player)) {
        case 0:
        default:
            screen = &gStatsSaveDoneMenuDesc1;
            break;
        case 1:
            screen = &gStatsSaveDoneMenuDesc2;
            break;
        case 2:
            screen = &gStatsSaveDoneMenuDesc3;
            break;
        case 3:
            screen = &gStatsSaveDoneMenuDesc4;
            break;
        }
        MenuPool_OpenMenu(player->m_pCamera->m_MenuPool, screen, player, player->m_ContState.m_pContState, 0);
    }
}
