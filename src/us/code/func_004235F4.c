#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 MenuItem_TextLength(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */

extern char sDamagedPakMenuText_BeSaved[];
extern char sDamagedPakMenuText_ControllerX[];

static __inline__ s32 GetPort(Menu *event) {
    if (gPakMenuAnyPak != 0) {
        return gPakMenuPort;
    }
    return event->controller->port;
}

s32 DamagedPakMenu_UpdateLine3(MenuItem *item, Menu *event) {
    s32 port = GetPort(event);

    if (sDamagedPakInGame != 0) {
        item->text = sDamagedPakMenuText_BeSaved;
    } else {
        item->text = sDamagedPakMenuText_ControllerX;
        sprintf(item->text + (MenuItem_TextLength(item) - 2), "%1d.", port + 1);
    }
    return 0;
}
