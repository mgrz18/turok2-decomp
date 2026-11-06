/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004449FC.c, func_004449FC, with Turok 2's layouts. */
#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char D_800B4008[];
extern char D_800B4020[];
extern char D_800B4038[];
extern char D_800C1310[]; /* number format */
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

s32 OptionsMenu_OpacityText(MenuItem *a) {
    u32 h = gEngineApp_m_Settings.hudAlphaSetting.value >> 4;

    if (h == 0) {
        a->text = D_800B4008;
    } else if (h == 0x10) {
        a->text = D_800B4020;
    } else {
        a->text = D_800B4038;
        sprintf(a->text + (MenuItem_TextLength(a) - 2), D_800C1310, h);
    }
    return 0;
}
