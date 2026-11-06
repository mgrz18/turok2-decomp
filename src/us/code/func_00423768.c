/* WRONG CUT: one function 0x00423768-0x00423880 (size 0x118); splat cut its second half into
 * func_004237FC (which starts in the delay slot of a bnel). */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

s32 PakMenu_CheckPak(void *event, s32 port, s32 arg2);
void MenuPool_OpenMenu(char *dst, char *fmt, CPlayer *player, s32 arg3, char *arg4);

extern char D_0043BED0[];
extern char D_0043BEF4[];
extern char D_0043C1A0[];
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

s32 DamagedPakMenu_SelectRetry(s32 unused, Menu *event) {
    s32 port = GetPort(event);
    char *dst;

    D_801309BC = 1;
    if (PakMenu_CheckPak(event, port, 1) != 0) {
        if (event->player != NULL) {
            dst = (char *)event->player->m_pCamera + 0x53C;
        } else {
            dst = D_80119E2C;
        }
        if (event->context == D_0043BEF4 || event->context == D_0043BED0) {
            D_80130940 = 0;
        }
        MenuPool_OpenMenu(dst, event->context, event->player, (s32) event->controller, NULL);
        return 1;
    } else {
        if (event->context == D_0043BEF4 || event->context == D_0043BED0) {
            D_80130940 = 0;
        }
        MenuPool_OpenMenu(D_80119E2C, event->context, event->player, (s32) event->controller, NULL);
        return 1;
    }
}
