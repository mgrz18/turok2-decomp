/* Second-module copy of PakMenu_UpdateModeText (first module; this one at link address 0x00421B04), calling this module's own functions at their link addresses. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"

s32 MenuItem_TextLength_2(void *item);
s32 sprintf(char *dst, const char *fmt, ...);


s32 PakMenu_UpdateModeText_2(MenuItem *item) {
    s32 player;

    if (gPakMenuIsProfile != 0 && gPakMenuIsSave != 0) {
        item->text = sPakMenuText_SaveCharacter;
    } else if (gPakMenuIsProfile == 0 && gPakMenuIsSave != 0) {
        item->text = sPakMenuText_SaveGame;
    } else if (gPakMenuAnyPak != 0) {
        player = gPakMenuPort;
        item->text = sPakMenuText_ControllerPakX_B48F8;
        sprintf(item->text + (MenuItem_TextLength_2(item) - 1), sPakMenuFmt_1d, player + 1);
    }
    return 0;
}
