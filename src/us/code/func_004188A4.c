/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/MISC/8043DF84_2.CPP, func_8043E6A4_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"
#include "globals/menu.h"
#include "globals/menu_data.h"

extern s32 Menu_StepValue(void *menu, s32 value, s32 step, s32 minimum, s32 maximum, s32 wrap);
extern s32 Controller_MenuRight(s32);
extern s32 Controller_ConfirmPressed(s32);

/* Steps the selection (twice when the controller reports busy) and sets the scale
 * from the table plus one, clamped to [0.5, 2.0]. */
s32 MpMenu_StepPlayerSpeed(void *arg0, Menu *item) {
    f32 scale;

    gMpPlayerSpeed = Menu_StepValue(item, gMpPlayerSpeed, 1, 0, 15, 0);
    if (Controller_MenuRight(item->controller) != 0 || Controller_ConfirmPressed(item->controller) != 0) {
        Menu_StepValue(item, gMpPlayerSpeed, 1, 0, 15, 0);
    }
    scale = sPlayerSpeedTable.scale[gMpPlayerSpeed] + 1.0f;
    if (!(scale < 0.5f)) {
        if (!(scale > 2.0f)) {
            if (scale < 0.5f) {
                scale = 0.5f;
            }
        } else {
            scale = 2.0f;
        }
    } else {
        scale = 0.5f;
    }
    gEngineApp_m_Settings.speed = scale;
    return 0;
}
