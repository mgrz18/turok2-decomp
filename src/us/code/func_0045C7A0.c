/* Second-module copy of DamagedPakMenu_SelectContinue (first module; this one at link address 0x004237A0), calling this module's own functions at their link addresses. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/pak.h"

void MenuPool_OpenMenu_2(char *dst, char *fmt, void *arg2, s32 arg3, s32 arg4);

extern char gSavePortalMenuDesc_2[];
extern char gSavePortalMenuDescB_2[];
extern char gEngineApp_m_Game_m_MenuPool[];
extern s32 gPakCheckDone;
extern s32 gCameraPoolHideViews;
extern s32 gPakMenuAnyPak;

s32 DamagedPakMenu_SelectContinue_2(s32 unused, Menu *event) {
    char *dst;

    gDamagedPakContinued = 1;
    gCameraPoolHideViews = 0;
    if (gPakMenuAnyPak != 0) {
        gPakCheckDone = 1;
    } else {
        if (event->player != NULL) {
            dst = (char *)event->player->m_pCamera + 0x53C;
        } else {
            dst = gEngineApp_m_Game_m_MenuPool;
        }
        if (event->text == gSavePortalMenuDescB_2 || event->text == gSavePortalMenuDesc_2) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu_2(dst, event->text, event->player, event->controller, 0);
    }
    return 1;
}
