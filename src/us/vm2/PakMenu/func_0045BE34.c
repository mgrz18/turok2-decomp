#include "common.h"
#include "protos/pak_2.h"
#include "structs/menu.h"
#include "globals/pak.h"
#include "structs/pak.h"
#include "protos/pak.h"



static __inline__ s32 GetPort(Menu *event) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return event->controller->port;
}

/* The pak selection (as in PakMenu_InitSaveGame_2): a two-word struct, so GCC keeps it in a stack slot; the store of
 * the port and the frame's extra 8 bytes come from it, while the call reads the register. */

void SavePortalMenu_Init_2(Menu *event) {
    ControllerProfile *controller = event->controller;
    PakChannel sel;

    gPakMenuAnyPak = 0;
    gDamagedPakContinued = 0;
    sSavePortalDone = 0;
    gSavePortalActive = 1;
    gPakMenuIsSave = 1;
    gPakMenuWrites = 1;
    gPakMenuIsLoad = 0;
    gPakMenuIsProfile = 0;
    sSavePortalChosen = 0;
    sel.channel = controller->port;
    Controller_WatchPak(sel.channel);
    Pak_ProbeSlot_2(GetPort(event));
}
