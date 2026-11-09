#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "structs/menu.h"
#include "globals/pak.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
#include "structs/player.h"
#include "structs/camera.h"


extern s32 gEngineApp_m_Game_m_PlayerList_m_bCinema[2]; /* local type: the field's real type (or the member path) changes the ROM's code here */

static __inline__ void ShowPlayerMessage(u8 *player, char *fmt) {
    MenuPool_OpenMenu((char *)((CPlayer *)player)->m_pCamera + T2_OFFSETOF(CCamera, m_MenuPool), fmt, player, (sptr)((CPlayer *)player)->m_ContState.m_pContState, 0);
}

s32 SavePortalMenu_SelectGoBack(s32 unused, Menu *event) {
    u8 *player;
    s32 port;
    s32 *p = &port; /* the ROM stores the port through its address and never reads it */

    sSavePortalChosen = 1;
    gEngineApp_m_Game_m_PlayerList_m_bCinema[0] = 0;
    gEngineApp_m_Game_m_PlayerList_m_bCinema[1] = 0;
    gCameraPoolHideViews = 0;
    player = NULL;
    if (gPakMenuAnyPak != 0) {
        *p = gPakMenuPort;
    } else {
        player = event->player;
        *p = event->controller->port;
    }
    ShowPlayerMessage(player, (char *)&gPakMsgInsertRumblePakDesc);
    gSavePortalActive = 0;
    return 1;
}
