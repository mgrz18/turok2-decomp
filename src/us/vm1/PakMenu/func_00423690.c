#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"

s32 PakMenu_CheckPak(void *event, s32 port, s32 arg2);

static __inline__ s32 GetPort(Menu *event) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return event->controller->port;
}

s32 DamagedPakMenu_SelectRepair(s32 unused, Menu *event) {
    if (PakMenu_CheckPak(event, GetPort(event), 1) != 0) {
        gPakMenuLeaving = 1;
        if (event->context == (char *)&gSavePortalMenuDescB || event->context == (char *)&gSavePortalMenuDesc) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, event->context, event->player, (sptr) event->controller, NULL);
        return 1;
    } else {
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakConfirmRepairDesc, event->player, (sptr) event->controller, event->context);
        return 1;
    }
}
