/* Second-module copy of func_00423690 (first module; this one at link address 0x004235B0), calling this module's own functions at their link addresses. */
#include "common.h"

s32 func_0041DCB0(void *event, s32 port, s32 arg2);
void func_00416644(char *dst, char *fmt, u8 *player, s32 arg3, char *arg4);

extern char D_0043BDF0[];
extern char D_0043BE14[];
extern char D_0043C0C0[];
extern char D_80119E2C[];
extern s32 D_800C2028;
extern s32 D_80130940;
extern s32 D_80130990;
extern s32 D_801309BC;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s8 port;
} MenuController;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ u8 *player;
    /* 0x20 */ MenuController *controller;
    /* 0x24 */ char *text;
} MenuEvent;

static __inline__ s32 GetPort(MenuEvent *event) {
    if (D_80130990 != 0) {
        return D_800C2028;
    }
    return event->controller->port;
}

s32 func_0045C5B0(s32 unused, MenuEvent *event) {
    if (func_0041DCB0(event, GetPort(event), 1) != 0) {
        D_801309BC = 1;
        if (event->text == D_0043BE14 || event->text == D_0043BDF0) {
            D_80130940 = 0;
        }
        func_00416644(D_80119E2C, event->text, event->player, (s32) event->controller, NULL);
        return 1;
    } else {
        func_00416644(D_80119E2C, D_0043C0C0, event->player, (s32) event->controller, event->text);
        return 1;
    }
}
