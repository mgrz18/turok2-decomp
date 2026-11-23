/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004449FC.c, func_004449FC, with Turok 2's layouts. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

s32 OptionsMenu_OpacityText(MenuItem *a) {
    u32 h = gEngineApp_m_Settings.hudAlphaSetting.value >> 4;

    if (h == 0) {
        a->text = sOptionsMenuText_OpacityClear;
    } else if (h == 0x10) {
        a->text = sOptionsMenuText_OpacityOpaque;
    } else {
        a->text = sOptionsMenuText_Opacity;
        sprintf(a->text + (MenuItem_TextLength(a) - 2), "%02d", h);
    }
    return 0;
}
