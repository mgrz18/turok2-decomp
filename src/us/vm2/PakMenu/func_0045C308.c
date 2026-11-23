/* Second-module copy of SavePortalMenu_SelectGoBack (first module). The port comes back through a pointer (a stack slot the ROM stores but never reads). */
#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "structs/menu.h"
#include "globals/pak.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"

void MenuPool_OpenMenu_2(char *dst, char *fmt, void *player, sptr controller, sptr arg4);

extern s32 gEngineApp_m_Game_m_PlayerList_m_bCinema[2]; /* local type: the field's real type (or the member path) changes the ROM's code here */

static __inline__ u8 *GetPlayer(Menu *event, s32 *port) {
    u8 *player = NULL;

    if (gPakMenuAnyPak != 0) {
        *port = gPakMenuPort;
    } else {
        player = event->player;
        *port = event->controller->port;
    }
    return player;
}

s32 SavePortalMenu_SelectGoBack_2(s32 unused, Menu *event) {
    u8 *player;
    s32 port;

    sSavePortalChosen = 1;
    gEngineApp_m_Game_m_PlayerList_m_bCinema[0] = 0;
    gEngineApp_m_Game_m_PlayerList_m_bCinema[1] = 0;
    gCameraPoolHideViews = 0;
    player = GetPlayer(event, &port);
    MenuPool_OpenMenu_2((char *)((CPlayer *)player)->m_pCamera->m_MenuPool, (char *)&gPakMsgInsertRumblePakDesc_2, player, (sptr)((CPlayer *)player)->m_ContState.m_pContState, 0);
    gSavePortalActive = 0;
    return 1;
}
