#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/game.h"
#include "globals/menu.h"

extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, const char *fmt, ...);

s32 MpMenu_PlayerSpeedLabel(MenuItem *a) {
    if (gEngineApp_m_Settings.speed == 1.0f) {
        a->text = sMpMenuText_PlayerSpeedDefault;
    } else {
        a->text = sMpMenuText_PlayerSpeed;
        sprintf(a->text + (MenuItem_TextLength(a) - 3), sMpMenuFmt_3d, gMpPlayerSpeed - 5);
    }
    return 0;
}
