/* Second-module copy of SavePortalMenu_Poll (first module; this one at link address 0x00422F60), calling this module's own functions at their link addresses. */
#include "common.h"
#include "structs/menu.h"

s32 Controller_PakChanged(s32 port);
s32 func_0041DCB0(void *event, s32 port, s32 arg2);

extern s32 D_800C2028;
extern s32 D_80130980;
extern s32 D_80130990;
extern s32 D_801309AC;

static __inline__ s32 GetPort(Menu *event) {
    if (D_80130990 != 0) {
        return D_800C2028;
    }
    return event->controller->port;
}

s32 SavePortalMenu_Poll_2(Menu *event) {
    if (D_80130980 == 0 && D_801309AC == 0) {
        if (GetPort(event) != -1 && Controller_PakChanged(GetPort(event)) != 0) {
            if (func_0041DCB0(event, GetPort(event), 0) != 0) {
                D_801309AC = 1;
            }
        }
    }
    return D_801309AC;
}
