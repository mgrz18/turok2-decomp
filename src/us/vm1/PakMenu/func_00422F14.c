#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"
#include "structs/pak.h"

void Controller_WatchPak(s32 port);
void Pak_ProbeSlot(s32 port);

extern s32 D_801309AC;

static __inline__ s32 GetPort(Menu *event) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return event->controller->port;
}

/* The pak selection (as in PakMenu_InitSaveGame): a two-word struct, so GCC keeps it in a stack slot; the store of
 * the port and the frame's extra 8 bytes come from it, while the call reads the register. */

void SavePortalMenu_Init(Menu *event) {
    ControllerProfile *controller = event->controller;
    PakChannel sel;

    gPakMenuAnyPak = 0;
    gDamagedPakContinued = 0;
    D_801309AC = 0;
    gSavePortalActive = 1;
    gPakMenuIsSave = 1;
    gPakMenuWrites = 1;
    gPakMenuIsLoad = 0;
    gPakMenuIsProfile = 0;
    sSavePortalChosen = 0;
    sel.channel = controller->port;
    Controller_WatchPak(sel.channel);
    Pak_ProbeSlot(GetPort(event));
}
