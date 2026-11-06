/* Second-module copy of PakMenu_UpdateModeText (first module; this one at link address 0x00421B04), calling this module's own functions at their link addresses. */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 MenuItem_TextLength_2(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800C1410[];
extern s32 gPakMenuPort;
extern char D_800B47F4[];
extern char D_800B4804[];
extern char D_800B48F8[];

s32 PakMenu_UpdateModeText_2(MenuItem *item) {
    s32 player;

    if (gPakMenuIsProfile != 0 && gPakMenuIsSave != 0) {
        item->text = D_800B47F4;
    } else if (gPakMenuIsProfile == 0 && gPakMenuIsSave != 0) {
        item->text = D_800B4804;
    } else if (gPakMenuAnyPak != 0) {
        player = gPakMenuPort;
        item->text = D_800B48F8;
        sprintf(item->text + (MenuItem_TextLength_2(item) - 1), D_800C1410, player + 1);
    }
    return 0;
}
