#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"
#include "globals/menu.h"

extern char sMpMenuText_PlayerSpeed[];
extern char sMpMenuText_PlayerSpeedDefault[];
extern char sMpMenuFmt_3d[];  /* "%3d" */
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

s32 MpMenu_PlayerSpeedLabel(MenuItem *a) {
    if (gEngineApp_m_Settings.speed == 1.0f) {
        a->text = sMpMenuText_PlayerSpeedDefault;
    } else {
        a->text = sMpMenuText_PlayerSpeed;
        sprintf(a->text + (MenuItem_TextLength(a) - 3), sMpMenuFmt_3d, gMpPlayerSpeed - 5);
    }
    return 0;
}
