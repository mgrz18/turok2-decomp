#include "common.h"
#include "structs/camera.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
#include "protos/player.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define P_S16(p, off) (*(s16 *)((u8 *)(p) + (off)))

/* Multiplayer/menu state: byte 0x1281 (gEngineApp_m_Settings_multiplayer) is the "enabled" flag. */

extern void CPlayer__StopLoopingSounds(void);

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
            text = (char *)&gStatsContinueMenuDesc1;
            break;
        case 1:
            text = (char *)&gStatsContinueMenuDesc2;
            break;
        case 2:
            text = (char *)&gStatsContinueMenuDesc3;
            break;
        case 3:
            text = (char *)&gStatsContinueMenuDesc4;
            break;
        }
        MenuPool_OpenMenu((u8 *)player->m_pCamera + T2_OFFSETOF(CCamera, m_MenuPool), text, player, (sptr) player->m_ContState.m_pContState, NULL);
    } else {
        text = (char *)&gGameOverMenuDesc;
        if (P_S16(player, 0x52A) != 0) {
            text = (char *)&gContinueMenuDesc;
        }
        MenuPool_OpenMenu((u8 *)player->m_pCamera + T2_OFFSETOF(CCamera, m_MenuPool), text, player, (sptr) player->m_ContState.m_pContState, NULL);
    }
}
