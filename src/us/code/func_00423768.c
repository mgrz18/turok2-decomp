/* WRONG CUT: one function 0x00423768-0x00423880 (size 0x118); splat cut its second half into
 * func_004237FC (which starts in the delay slot of a bnel). */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/pak.h"

s32 PakMenu_CheckPak(void *event, s32 port, s32 arg2);
void MenuPool_OpenMenu(char *dst, char *fmt, CPlayer *player, s32 arg3, char *arg4);

extern char gSavePortalMenuDesc[];
extern char gSavePortalMenuDescB[];
extern char gPakConfirmRepairDesc[];
extern s32 gPakMenuPort;
extern s32 gCameraPoolHideViews;

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
            dst = (char *)event->player->m_pCamera + 0x53C;
        } else {
            dst = &gEngineApp_m_Game_m_MenuPool;
        }
        if (event->context == gSavePortalMenuDescB || event->context == gSavePortalMenuDesc) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu(dst, event->context, event->player, (s32) event->controller, NULL);
        return 1;
    } else {
        if (event->context == gSavePortalMenuDescB || event->context == gSavePortalMenuDesc) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, event->context, event->player, (s32) event->controller, NULL);
        return 1;
    }
}
