#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"
#include "globals/menu.h"

void MenuPool_OpenMenu(char *dst, char *fmt, void *arg2, s32 arg3, s32 arg4);

extern char gPakMsgInsertRumblePakDesc[];
extern s32 gPakMenuPort;
extern s32 gEngineApp_m_Game_m_PlayerList_m_bCinema[2];

static __inline__ void ShowPlayerMessage(u8 *player, char *fmt) {
    MenuPool_OpenMenu(*(char **)(player + 0x51C) + 0x53C, fmt, player, *(s32 *)(player + 0xA50), 0);
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
    ShowPlayerMessage(player, gPakMsgInsertRumblePakDesc);
    gSavePortalActive = 0;
    return 1;
}
