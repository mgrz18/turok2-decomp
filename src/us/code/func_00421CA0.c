#include "common.h"
#include "structs/menu.h"

s32 MenuItem_TextLength(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800C140C[];
extern char D_800C1410[];
extern char D_800B45E0[];
extern s32 gPakMenuAnyPak;

s32 PakMenu_UpdateTitle(MenuItem *item, Menu *event) {
    s32 port;

    if (gPakMenuAnyPak != 0) {
        item->text = D_800C140C;
    } else {
        port = event->controller->port;
        item->text = D_800B45E0;
        sprintf(item->text + (MenuItem_TextLength(item) - 1), D_800C1410, port + 1);
    }
    return 0;
}
