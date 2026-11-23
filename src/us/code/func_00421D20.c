/* WRONG CUT: one function 0x00421D20-0x00421DFC (size 0xDC); splat cut its second half into
 * func_00421D90 (it starts in the middle of an if chain, reading $v0 set here). */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern char D_800C140C[];
extern char sPakMenuText_LoadNote_B4870[];
extern char sPakMenuText_SaveNote_B487C[];
extern char sPakMenuText_LoadNote_B4888[];
extern char sPakMenuText_SaveNote_B4894[];
extern char sPakMenuText_DeleteNote[];
extern char sPakMenuText_ControllerPakMenu[];

s32 PakMenu_UpdateSubtitle(MenuItem *item, s16 *id) {
    s32 n = *id;
    u32 k = n - 3;

    if (gPakMenuIsLoad != 0) {
        if (gPakMenuIsProfile != 0) {
            item->text = sPakMenuText_LoadNote_B4870;
        } else {
            item->text = sPakMenuText_LoadNote_B4888;
        }
    } else if (gPakMenuIsSave != 0) {
        if (k < 16) {
            item->text = sPakMenuText_DeleteNote;
            return 0;
        }
        if (n == 21) {
            item->text = D_800C140C;
        } else if (gPakMenuIsProfile != 0) {
            item->text = sPakMenuText_SaveNote_B487C;
        } else {
            item->text = sPakMenuText_SaveNote_B4894;
        }
    } else if (gPakMenuAnyPak != 0) {
        item->text = sPakMenuText_ControllerPakMenu;
    }
    return 0;
}
