/* Second-module copy of SavePortalMenu_InitB (first module). The port comes back through a pointer (a stack slot), which gives the ROM's store-then-reload. */
#include "common.h"
#include "structs/menu.h"

void Controller_WatchPak(s32 port);

extern s32 D_800C2020;
extern s32 D_800C2028;
extern s32 D_80130980;
extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_80130998;
extern s32 D_801309AC;
extern s32 D_801309B4;

static __inline__ void GetPort(Menu *event, s32 *port) {
    if (D_80130990 != 0) {
        *port = D_800C2028;
    } else {
        *port = event->controller->port;
    }
}

void SavePortalMenu_InitB_2(Menu *event) {
    s32 port;

    D_801309AC = 0;
    D_80130998 = 1;
    D_80130984 = 1;
    D_801309B4 = 0;
    D_80130980 = 0;
    GetPort(event, &port);
    D_800C2020 = 0;
    Controller_WatchPak(port);
}
