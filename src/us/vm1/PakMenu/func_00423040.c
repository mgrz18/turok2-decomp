#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"
#include "protos/pak.h"

s32 PakMenu_CheckPak(void *event, s32 port, s32 arg2);


static __inline__ s32 GetPort(Menu *event) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return event->controller->port;
}

s32 SavePortalMenu_Poll(Menu *event) {
    if (sSavePortalChosen == 0 && sSavePortalDone == 0) {
        if (GetPort(event) != -1 && Controller_PakChanged(GetPort(event)) != 0) {
            if (PakMenu_CheckPak(event, GetPort(event), 0) != 0) {
                sSavePortalDone = 1;
            }
        }
    }
    return sSavePortalDone;
}
