/* Second-module copy of PakConfirmMenu_SelectNo (first module). NOTE: the ROM code reads jtbl_800C1450, whose entries are the first module copy's case addresses (link base 0x420F34, 0xE0 above this copy's 0x420E54): the switch here builds its own table, which will not equal the ROM's. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"

s32 PakMenu_CheckPak_2(void *event, s32 port, s32 arg2);
char *PakMenu_GetReturnMenu_2(void *event);
void MenuPool_OpenMenu_2(char *dst, char *fmt, CPlayer *player, s32 arg3, char *arg4);


static __inline__ s32 GetPort(Menu *event) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return event->controller->port;
}

static __inline__ void ShowEvent(Menu *event) {
    if (PakMenu_CheckPak_2(event, GetPort(event), 1) != 0) {
        gPakMenuLeaving = 1;
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, event->context, event->player, (s32) event->controller, NULL);
    } else {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, event->context, event->player, (s32) event->controller, NULL);
    }
}

s32 PakConfirmMenu_SelectNo_2(s32 unused, Menu *event) {
    char *text;
    char *dst;

    switch (gPakConfirmKind) {
        case 4:
            ShowEvent(event);
            break;
        case 3:
            ShowEvent(event);
            break;
        case 0:
            ShowEvent(event);
            break;
        case 1:
        case 2:
            gPakMenuLeaving = 1;
            gCameraPoolHideViews = 0;
            if (gPakMenuAnyPak != 0) {
                gPakCheckDone = 1;
                return 1;
            }
            text = PakMenu_GetReturnMenu_2(event);
            if (text == NULL) {
                text = event->context;
            }
            if (event->context == &gSavePortalMenuDescB_2 || event->context == &gSavePortalMenuDesc_2) {
                gCameraPoolHideViews = 0;
            }
            if (event->player != NULL) {
                dst = (char *)event->player->m_pCamera + 0x53C;
            } else {
                dst = &gEngineApp_m_Game_m_MenuPool;
            }
            MenuPool_OpenMenu_2(dst, text, event->player, (s32) event->controller, NULL);
            break;
    }
    return 1;
}
