#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

void Controller_WatchPak(s32 port);

extern s32 gDamagedPakContinued;
extern s32 gPakMenuPort;
extern s32 D_801309AC;

/* Takes the port by address: the ROM keeps it in a stack slot (as PakMenu_GetReturnMenu does with its kind). */
static __inline__ void SelectPort(s32 *port) {
    gDamagedPakContinued = 0;
    Controller_WatchPak(*port);
}

void SavePortalMenu_InitB(Menu *event) {
    s32 port;

    D_801309AC = 0;
    gSavePortalActive = 1;
    gPakMenuIsSave = 1;
    gPakMenuIsProfile = 0;
    sSavePortalChosen = 0;
    if (gPakMenuAnyPak != 0) {
        port = gPakMenuPort;
    } else {
        port = event->controller->port;
    }
    SelectPort(&port);
}
