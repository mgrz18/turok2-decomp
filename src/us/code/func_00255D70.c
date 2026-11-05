#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/camera.h"

extern char D_0043ABF0;
extern char D_0043AC14;
extern char D_0043AC38;
extern char D_0043AC5C;

void func_0042EB04(CPlayer *);
s32 func_0025340C(CPlayerList *players, CPlayer *player);
void func_00416644(void *pool, void *screen, CPlayer *p, void *controller, s32);

/* func_0042EB04 takes the player (as in func_00249118): the call keeps it in $a0,
 * which is what lets sched2 keep the mask's lui/ori together. */
/* Clears the player's flag 0x800000 and, in multiplayer, opens the screen of
 * the player's slot (one of four) on the player's camera. */
void func_00255D70(CPlayer *player) {
    char *screen;

    player->m_ShockedTimer = 0;
    player->m_CharacterActor.m_dwFlags &= ~0x800000;
    func_0042EB04(player);
    if (D_80119870.m_Settings.multiplayer) {
        switch ((u32)func_0025340C(&D_80119870.m_PlayerList, player)) {
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
        func_00416644(player->m_pCamera->m_MenuPool, screen, player, player->m_ContState.m_pContState, 0);
    }
}
