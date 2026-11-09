/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00444F0C.c, func_00444F0C, with Turok 2's layouts. */
#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"
#include "protos/cache.h"

extern char D_800B4158[];
extern char D_800B4170[];
extern char D_800C1318[];
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

s32 OptionsMenu_BrightnessText(MenuItem *a) {
    u32 h = gEngineApp_m_Settings.unkC >> 4;

    CCache__SetFrameDelay(CCache__GetFrameDelay() + 2);
    if (h == 0) {
        a->text = D_800B4158;
    } else {
        a->text = D_800B4170;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), D_800C1318, h);
    }
    return 0;
}
