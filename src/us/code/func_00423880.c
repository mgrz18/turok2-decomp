#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/pak.h"

void MenuPool_OpenMenu(char *dst, char *fmt, void *arg2, s32 arg3, s32 arg4);

extern char gSavePortalMenuDesc[];
extern char gSavePortalMenuDescB[];

s32 DamagedPakMenu_SelectContinue(s32 unused, Menu *event) {
    char *dst;

    gDamagedPakContinued = 1;
    gCameraPoolHideViews = 0;
    if (gPakMenuAnyPak != 0) {
        gPakCheckDone = 1;
    } else {
        if (event->player != NULL) {
            dst = (char *)event->player->m_pCamera + 0x53C;
        } else {
            dst = &gEngineApp_m_Game_m_MenuPool;
        }
        if (event->text == gSavePortalMenuDescB || event->text == gSavePortalMenuDesc) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu(dst, event->text, event->player, event->controller, 0);
    }
    return 1;
}
