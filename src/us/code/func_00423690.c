#include "common.h"
#include "structs/menu.h"

s32 PakMenu_CheckPak(void *event, s32 port, s32 arg2);
void MenuPool_OpenMenu(char *dst, char *fmt, u8 *player, s32 arg3, char *arg4);

extern char gSavePortalMenuDesc[];
extern char gSavePortalMenuDescB[];
extern char gPakConfirmRepairDesc[];
extern char gEngineApp_m_Game_m_MenuPool[];
extern s32 gPakMenuPort;
extern s32 gCameraPoolHideViews;
extern s32 gPakMenuAnyPak;
extern s32 gPakMenuLeaving;

static __inline__ s32 GetPort(Menu *event) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return event->controller->port;
}

s32 DamagedPakMenu_SelectRepair(s32 unused, Menu *event) {
    if (PakMenu_CheckPak(event, GetPort(event), 1) != 0) {
        gPakMenuLeaving = 1;
        if (event->context == gSavePortalMenuDescB || event->context == gSavePortalMenuDesc) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, event->context, event->player, (s32) event->controller, NULL);
        return 1;
    } else {
        MenuPool_OpenMenu(gEngineApp_m_Game_m_MenuPool, gPakConfirmRepairDesc, event->player, (s32) event->controller, event->context);
        return 1;
    }
}
