#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 MenuItem_TextLength(void *item);
s32 sprintf(char *dst, const char *fmt, ...); /* sprintf */

extern char sPakMenuText_SaveCharacter[];
extern char sPakMenuText_SaveGame[];
extern char sPakMenuText_ControllerPakX_B48F8[];

s32 PakMenu_UpdateModeText(MenuItem *item) {
    s32 player;

    if (gPakMenuIsProfile != 0 && gPakMenuIsSave != 0) {
        item->text = sPakMenuText_SaveCharacter;
    } else if (gPakMenuIsProfile == 0 && gPakMenuIsSave != 0) {
        item->text = sPakMenuText_SaveGame;
    } else if (gPakMenuAnyPak != 0) {
        player = gPakMenuPort;
        item->text = sPakMenuText_ControllerPakX_B48F8;
        sprintf(item->text + (MenuItem_TextLength(item) - 1), "%1d", player + 1);
    }
    return 0;
}
