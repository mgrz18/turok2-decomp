/* WRONG CUT: one function 0x00421D20-0x00421DFC (size 0xDC); splat cut its second half into
 * func_00421D90 (it starts in the middle of an if chain, reading $v0 set here). */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern char D_800C140C[];
extern char D_800B4870[];
extern char D_800B487C[];
extern char D_800B4888[];
extern char D_800B4894[];
extern char D_800B48A8[];
extern char D_800B48E4[];

s32 PakMenu_UpdateSubtitle(MenuItem *item, s16 *id) {
    s32 n = *id;
    u32 k = n - 3;

    if (gPakMenuIsLoad != 0) {
        if (gPakMenuIsProfile != 0) {
            item->text = D_800B4870;
        } else {
            item->text = D_800B4888;
        }
    } else if (gPakMenuIsSave != 0) {
        if (k < 16) {
            item->text = D_800B48A8;
            return 0;
        }
        if (n == 21) {
            item->text = D_800C140C;
        } else if (gPakMenuIsProfile != 0) {
            item->text = D_800B487C;
        } else {
            item->text = D_800B4894;
        }
    } else if (gPakMenuAnyPak != 0) {
        item->text = D_800B48E4;
    }
    return 0;
}
