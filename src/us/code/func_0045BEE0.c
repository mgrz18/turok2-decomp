/* Second-module copy of SavePortalMenu_InitB (first module). The port comes back through a pointer (a stack slot), which gives the ROM's store-then-reload. */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

void Controller_WatchPak(s32 port);

extern s32 D_801309AC;

static __inline__ void GetPort(Menu *event, s32 *port) {
    if (gPakMenuAnyPak != 0) {
        *port = gPakMenuPort;
    } else {
        *port = event->controller->port;
    }
}

void SavePortalMenu_InitB_2(Menu *event) {
    s32 port;

    D_801309AC = 0;
    gSavePortalActive = 1;
    gPakMenuIsSave = 1;
    gPakMenuIsProfile = 0;
    sSavePortalChosen = 0;
    GetPort(event, &port);
    gDamagedPakContinued = 0;
    Controller_WatchPak(port);
}
