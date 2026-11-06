#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

extern s32 gMpPlayerSpeed;     /* the speed setting, 0-15 */
extern f32 sPlayerSpeedTable_scale_2[];   /* speed per setting */

s32 Menu_StepValue_2(Menu *menu, s32 value, s32 step, s32 minimum, s32 maximum, s32 wrap);
s32 Controller_MenuRight(s32 controller);
s32 Controller_ConfirmPressed(s32 controller);

#define MIN(a, b) ((b) < (a) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

/* Adjusts the game speed setting and stores its multiplier, clamped to [0.5, 2]. */
s32 MpMenu_StepPlayerSpeed_2(void *item, Menu *menu) {
    f32 speed;

    gMpPlayerSpeed = Menu_StepValue_2(menu, gMpPlayerSpeed, 1, 0, 15, 0);
    if (Controller_MenuRight(menu->controller) || Controller_ConfirmPressed(menu->controller)) {
        Menu_StepValue_2(menu, gMpPlayerSpeed, 1, 0, 15, 0);
    }
    speed = sPlayerSpeedTable_scale_2[gMpPlayerSpeed] + 1.0f;
    speed = MIN(MAX(speed, 0.5f), 2.0f);
    gEngineApp_m_Settings.speed = speed;
    return 0;
}
