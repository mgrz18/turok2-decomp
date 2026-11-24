#include "common.h"
#include "protos/pak_2.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
#include "protos/pakmenu.h"


static __inline__ s32 GetPort(Menu *event) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return event->controller->port;
}

s32 DamagedPakMenu_SelectRepair_2(s32 unused, Menu *event) {
    if (PakMenu_CheckPak_2((struct Menu *) event, GetPort(event), 1) != 0) {
        gPakMenuLeaving = 1;
        if (event->context == (char *)&gSavePortalMenuDescB_2 || event->context == (char *)&gSavePortalMenuDesc_2) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, event->context, event->player, (sptr) event->controller, NULL);
        return 1;
    } else {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, (char *)&gPakConfirmRepairDesc_2, event->player, (sptr) event->controller, event->context);
        return 1;
    }
}
