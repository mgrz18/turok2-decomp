/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00444F0C.c, func_00444F0C, with Turok 2's layouts. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/game.h"
#include "protos/cache.h"

extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

s32 OptionsMenu_BrightnessText(MenuItem *a) {
    u32 h = gEngineApp_m_Settings.unkC >> 4;

    CCache__SetFrameDelay(CCache__GetFrameDelay() + 2);
    if (h == 0) {
        a->text = sOptionsMenuText_BrightnessNorm;
    } else {
        a->text = sOptionsMenuText_Brightness;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sOptionsMenuFmt_02d, h);
    }
    return 0;
}
