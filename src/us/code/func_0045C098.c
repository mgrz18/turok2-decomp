/* Second-module copy of SavePortalMenu_SelectSave (first module; this one at link address 0x00423098), calling this module's own functions at their link addresses. */
#include "common.h"
#include "structs/menu.h"

s32 Cinema_IsActive(void);
s32 Cinema_IsFinished(void);
void MenuPool_OpenMenu_2(char *dst, char *fmt, s32 arg2, s32 arg3, s32 arg4);

extern char D_0043B634[];
extern char D_80119E2C[];
extern s32 D_80130980;

static __inline__ s32 func_00422EE0_inline(void) {
    if (Cinema_IsActive() != 0 && Cinema_IsFinished() == 0) {
        return 1;
    }
    return 0;
}

s32 SavePortalMenu_SelectSave_2(s32 unused, Menu *event) {
    s32 player = event->player;

    if (func_00422EE0_inline()) {
        return 0;
    }
    if (player != 0) {
        D_80130980 = 1;
        MenuPool_OpenMenu_2(D_80119E2C, D_0043B634, event->player, event->controller, 0);
        return 1;
    }
    return 0;
}
