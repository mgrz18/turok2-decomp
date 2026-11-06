#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 Controller_PakChanged(s32 port);
s32 PakMenu_CheckPak(void *event, s32 port, s32 arg2);

extern s32 sSavePortalChosen;
extern s32 gPakMenuAnyPak;
extern s32 D_801309AC;

static __inline__ s32 GetPort(Menu *event) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return event->controller->port;
}

s32 SavePortalMenu_Poll(Menu *event) {
    if (sSavePortalChosen == 0 && D_801309AC == 0) {
        if (GetPort(event) != -1 && Controller_PakChanged(GetPort(event)) != 0) {
            if (PakMenu_CheckPak(event, GetPort(event), 0) != 0) {
                D_801309AC = 1;
            }
        }
    }
    return D_801309AC;
}
