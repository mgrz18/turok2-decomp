#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"

s32 Cinema_IsActive(void);
s32 Cinema_IsFinished(void);
void MenuPool_OpenMenu(char *dst, char *fmt, s32 arg2, s32 arg3, s32 arg4);

extern char gPakSaveGameMenuDesc[];
extern s32 sSavePortalChosen;

static __inline__ s32 func_00422EE0_inline(void) {
    if (Cinema_IsActive() != 0 && Cinema_IsFinished() == 0) {
        return 1;
    }
    return 0;
}

s32 SavePortalMenu_SelectSave(s32 unused, Menu *event) {
    s32 player = event->player;

    if (func_00422EE0_inline()) {
        return 0;
    }
    if (player != 0) {
        sSavePortalChosen = 1;
        MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, gPakSaveGameMenuDesc, event->player, event->controller, 0);
        return 1;
    }
    return 0;
}
