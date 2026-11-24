#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 MenuItem_TextLength_2(void *item);
s32 sprintf(char *dst, const char *fmt, ...);


static __inline__ s32 GetPort(Menu *event) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return event->controller->port;
}

s32 DamagedPakMenu_UpdateLine3_2(MenuItem *item, Menu *event) {
    s32 port = GetPort(event);

    if (sDamagedPakInGame != 0) {
        item->text = sDamagedPakMenuText_BeSaved;
    } else {
        item->text = sDamagedPakMenuText_ControllerX;
        sprintf(item->text + (MenuItem_TextLength_2(item) - 2), "%1d.", port + 1);
    }
    return 0;
}
