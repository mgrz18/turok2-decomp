#include "common.h"
#include "structs/menu.h"

s32 func_0041DD90(void *event, s32 port, s32 arg2);
void func_00416644(char *dst, char *fmt, u8 *player, s32 arg3, char *arg4);

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

s32 func_00423690(s32 unused, Menu *event) {
    if (func_0041DD90(event, GetPort(event), 1) != 0) {
        D_801309BC = 1;
        if (event->context == D_0043BEF4 || event->context == D_0043BED0) {
            D_80130940 = 0;
        }
        func_00416644(D_80119E2C, event->context, event->player, (s32) event->controller, NULL);
        return 1;
    } else {
        func_00416644(D_80119E2C, D_0043C1A0, event->player, (s32) event->controller, event->context);
        return 1;
    }
}
