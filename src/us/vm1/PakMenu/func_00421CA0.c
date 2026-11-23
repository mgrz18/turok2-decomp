#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 MenuItem_TextLength(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800C140C[];

s32 PakMenu_UpdateTitle(MenuItem *item, Menu *event) {
    s32 port;

    if (gPakMenuAnyPak != 0) {
        item->text = D_800C140C;
    } else {
        port = event->controller->port;
        item->text = sPakMenuText_ControllerPakX_B45E0;
        sprintf(item->text + (MenuItem_TextLength(item) - 1), sPakMenuFmt_1d, port + 1);
    }
    return 0;
}
