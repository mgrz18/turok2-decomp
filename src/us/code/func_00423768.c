/* WRONG CUT: one function 0x00423768-0x00423880 (size 0x118); splat cut its second half into
 * func_004237FC (which starts in the delay slot of a bnel). */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
#include "structs/camera.h"

s32 PakMenu_CheckPak(void *event, s32 port, s32 arg2);

static __inline__ s32 GetPort(Menu *event) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return event->controller->port;
}

s32 DamagedPakMenu_SelectRetry(s32 unused, Menu *event) {
    s32 port = GetPort(event);
    char *dst;

    gPakMenuLeaving = 1;
    if (PakMenu_CheckPak(event, port, 1) != 0) {
        if (event->player != NULL) {
            dst = (char *)event->player->m_pCamera + T2_OFFSETOF(CCamera, m_MenuPool);
        } else {
            dst = &gEngineApp_m_Game_m_MenuPool;
        }
        if (event->context == &gSavePortalMenuDescB || event->context == &gSavePortalMenuDesc) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu(dst, event->context, event->player, (sptr) event->controller, NULL);
        return 1;
    } else {
        if (event->context == &gSavePortalMenuDescB || event->context == &gSavePortalMenuDesc) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, event->context, event->player, (sptr) event->controller, NULL);
        return 1;
    }
}
