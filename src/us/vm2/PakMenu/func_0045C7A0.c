#include "common.h"
#include "protos/pak_2.h"
#include "structs/camera.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
#include "structs/camera.h"


s32 DamagedPakMenu_SelectContinue_2(s32 unused, Menu *event) {
    char *dst;

    gDamagedPakContinued = 1;
    gCameraPoolHideViews = 0;
    if (gPakMenuAnyPak != 0) {
        gPakCheckDone = 1;
    } else {
        if (event->player != NULL) {
            dst = (char *)event->player->m_pCamera->m_MenuPool;
        } else {
            dst = &gEngineApp_m_Game_m_MenuPool;
        }
        if (event->text == (char *)&gSavePortalMenuDescB_2 || event->text == (char *)&gSavePortalMenuDesc_2) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu_2(dst, event->text, event->player, event->controller, 0);
    }
    return 1;
}
