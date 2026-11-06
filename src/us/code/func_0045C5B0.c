/* Second-module copy of DamagedPakMenu_SelectRepair (first module; this one at link address 0x004235B0), calling this module's own functions at their link addresses. */
#include "common.h"
#include "structs/menu.h"

s32 PakMenu_CheckPak_2(void *event, s32 port, s32 arg2);
void MenuPool_OpenMenu_2(char *dst, char *fmt, u8 *player, s32 arg3, char *arg4);

extern char D_0043BDF0[];
extern char D_0043BE14[];
extern char D_0043C0C0[];
extern char D_80119E2C[];
extern s32 D_800C2028;
extern s32 D_80130940;
extern s32 D_80130990;
extern s32 D_801309BC;

static __inline__ s32 GetPort(Menu *event) {
    if (D_80130990 != 0) {
        return D_800C2028;
    }
    return event->controller->port;
}

s32 DamagedPakMenu_SelectRepair_2(s32 unused, Menu *event) {
    if (PakMenu_CheckPak_2(event, GetPort(event), 1) != 0) {
        D_801309BC = 1;
        if (event->context == D_0043BE14 || event->context == D_0043BDF0) {
            D_80130940 = 0;
        }
        MenuPool_OpenMenu_2(D_80119E2C, event->context, event->player, (s32) event->controller, NULL);
        return 1;
    } else {
        MenuPool_OpenMenu_2(D_80119E2C, D_0043C0C0, event->player, (s32) event->controller, event->context);
        return 1;
    }
}
