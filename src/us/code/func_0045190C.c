#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"
#include "globals/menu.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char sMpMenuText_PlayerSpeed[];
extern char sMpMenuText_PlayerSpeedDefault[];
extern char sMpMenuFmt_3d[];  /* "%3d", earlier in the file */

/* Names the game speed: a fixed line at 1.0, otherwise the setting relative to 5. */
s32 MpMenu_PlayerSpeedLabel_2(MenuItem *item) {
    if (gEngineApp_m_Settings.speed == 1.0f) {
        item->text = sMpMenuText_PlayerSpeedDefault;
    } else {
        item->text = sMpMenuText_PlayerSpeed;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 3], sMpMenuFmt_3d, gMpPlayerSpeed - 5);
    }
    return 0;
}
