/* WRONG CUT: the real function starts at 0x0045C688 (size 0x118 = 280): splat
 * cut its first two words (lui/lw gPakMenuAnyPak) as DamagedPakMenu_SelectRetry_2. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/menu_desc.h"

/* Start the selected level, then open the next menu page (on the player's own
 * menu when there is a player). */

extern s32 gPakMenuAnyPak;
extern s32 gPakMenuPort;
extern s32 gPakMenuLeaving;
extern s32 gCameraPoolHideViews;
extern u8 gEngineApp_m_Game_m_MenuPool[];

s32 PakMenu_CheckPak_2(Menu *event, s32 mode, s32 arg2);
void MenuPool_OpenMenu_2(void *menu, void *page, void *player, void *args, s32 flags);

static __inline__ s32 currentMode(Menu *event) {
    s32 mode;

    if (gPakMenuAnyPak != 0) {
        mode = gPakMenuPort;
    } else {
        mode = event->controller->port;
    }
    return mode;
}

s32 DamagedPakMenu_SelectRetry_2(void *arg0, Menu *event) {
    s32 mode = currentMode(event);
    void *menu;

    gPakMenuLeaving = 1;
    if (PakMenu_CheckPak_2(event, mode, 1) != 0) {
        if (event->player != NULL) {
            menu = (u8 *)event->player->m_pCamera + 0x53C;
        } else {
            menu = gEngineApp_m_Game_m_MenuPool;
        }
        if (event->context == &gSavePortalMenuDescB_2 || event->context == &gSavePortalMenuDesc_2) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu_2(menu, event->context, event->player, event->controller, 0);
        return 1;
    } else {
        if (event->context == &gSavePortalMenuDescB_2 || event->context == &gSavePortalMenuDesc_2) {
            gCameraPoolHideViews = 0;
        }
        MenuPool_OpenMenu_2(gEngineApp_m_Game_m_MenuPool, event->context, event->player, event->controller, 0);
        return 1;
    }
}
