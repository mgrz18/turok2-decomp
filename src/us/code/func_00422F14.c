#include "common.h"
#include "structs/menu.h"

void Controller_WatchPak(s32 port);
void Pak_ProbeSlot(s32 port);

extern s32 D_800C2020;
extern s32 D_800C2028;
extern s32 D_80130980;
extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_80130994;
extern s32 D_80130998;
extern s32 D_801309A0;
extern s32 D_801309AC;
extern s32 D_801309B4;

static __inline__ s32 GetPort(Menu *event) {
    if (D_80130990 != 0) {
        return D_800C2028;
    }
    return event->controller->port;
}

/* The pak selection (as in PakMenu_InitSaveGame): a two-word struct, so GCC keeps it in a stack slot; the store of
 * the port and the frame's extra 8 bytes come from it, while the call reads the register. */
typedef struct {
    s32 v;
    s32 w;
} PadSel;

void SavePortalMenu_Init(Menu *event) {
    ControllerProfile *controller = event->controller;
    PadSel sel;

    D_80130990 = 0;
    D_800C2020 = 0;
    D_801309AC = 0;
    D_80130998 = 1;
    D_80130984 = 1;
    D_801309A0 = 1;
    D_80130994 = 0;
    D_801309B4 = 0;
    D_80130980 = 0;
    sel.v = controller->port;
    Controller_WatchPak(sel.v);
    Pak_ProbeSlot(GetPort(event));
}
