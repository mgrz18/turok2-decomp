#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define P_S16(p, off) (*(s16 *)((u8 *)(p) + (off)))

/* Multiplayer/menu state: byte 0x1281 (gEngineApp_m_Settings_multiplayer) is the "enabled" flag. */

extern char gContinueMenuDesc[];
extern char gGameOverMenuDesc[];
extern char gStatsContinueMenuDesc1[];
extern char gStatsContinueMenuDesc2[];
extern char gStatsContinueMenuDesc3[];
extern char gStatsContinueMenuDesc4[];

extern void CPlayer__StopLoopingSounds(void);
extern s32 CPlayerPool__GetIndex(void *state, CPlayer *player);
extern void MenuPool_OpenMenu(void *a, char *text, CPlayer *player, s32 d, void *e);

void PlayerState15_Enter(CPlayer *player, CGameObjectInstance *actor) {
    char *text;

    actor->m_vVelocity.x = 0.0f;
    actor->m_vVelocity.y = 0.0f;
    actor->m_vVelocity.z = 0.0f;
    CPlayer__StopLoopingSounds();
    if (gEngineApp_m_Game.m_Settings.multiplayer != 0) {
        switch ((u32) CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, player)) {
        default:
        case 0:
            text = gStatsContinueMenuDesc1;
            break;
        case 1:
            text = gStatsContinueMenuDesc2;
            break;
        case 2:
            text = gStatsContinueMenuDesc3;
            break;
        case 3:
            text = gStatsContinueMenuDesc4;
            break;
        }
        MenuPool_OpenMenu((u8 *)P_S32(player, 0x51C) + 0x53C, text, player, P_S32(player, 0xA50), NULL);
    } else {
        text = gGameOverMenuDesc;
        if (P_S16(player, 0x52A) != 0) {
            text = gContinueMenuDesc;
        }
        MenuPool_OpenMenu((u8 *)P_S32(player, 0x51C) + 0x53C, text, player, P_S32(player, 0xA50), NULL);
    }
}
