#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 MenuItem_TextLength(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800C1410[];
extern s32 gPakMenuPort;
extern char D_800B47F4[];
extern char D_800B4804[];
extern char D_800B48F8[];

s32 PakMenu_UpdateModeText(MenuItem *item) {
    s32 player;

    if (gPakMenuIsProfile != 0 && gPakMenuIsSave != 0) {
        item->text = D_800B47F4;
    } else if (gPakMenuIsProfile == 0 && gPakMenuIsSave != 0) {
        item->text = D_800B4804;
    } else if (gPakMenuAnyPak != 0) {
        player = gPakMenuPort;
        item->text = D_800B48F8;
        sprintf(item->text + (MenuItem_TextLength(item) - 1), D_800C1410, player + 1);
    }
    return 0;
}
