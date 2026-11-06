/* Second-module copy of DamagedPakMenu_SelectRepair (first module; this one at link address 0x004235B0), calling this module's own functions at their link addresses. */
#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/pak.h"

s32 PakMenu_CheckPak_2(void *event, s32 port, s32 arg2);
void MenuPool_OpenMenu_2(char *dst, char *fmt, u8 *player, s32 arg3, char *arg4);

extern char gSavePortalMenuDesc_2[];
extern char gSavePortalMenuDescB_2[];
extern char gPakConfirmRepairDesc_2[];
extern s32 gPakMenuPort;
extern s32 gCameraPoolHideViews;

static __inline__ s32 GetPort(Menu *event) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return event->controller->port;
}

s32 DamagedPakMenu_SelectRepair_2(s32 unused, Menu *event) {
    if (PakMenu_CheckPak_2(event, GetPort(event), 1) != 0) {
        gPakMenuLeaving = 1;
        if (event->context == gSavePortalMenuDescB_2 || event->context == gSavePortalMenuDesc_2) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, event->context, event->player, (s32) event->controller, NULL);
        return 1;
    } else {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, gPakConfirmRepairDesc_2, event->player, (s32) event->controller, event->context);
        return 1;
    }
}
