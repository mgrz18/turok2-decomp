#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char sOptionsMenuText_OpacityClear[];
extern char sOptionsMenuText_OpacityOpaque[];
extern char sOptionsMenuText_Opacity[];

/* Shows the HUD alpha setting in sixteenths: two fixed lines for 0 and 16, otherwise the number. */
s32 OptionsMenu_OpacityText_2(MenuItem *item) {
    s32 value = gEngineApp_m_Settings.hudAlphaSetting.value >> 4;

    if (value == 0) {
        item->text = sOptionsMenuText_OpacityClear;
    } else if (value == 16) {
        item->text = sOptionsMenuText_OpacityOpaque;
    } else {
        item->text = sOptionsMenuText_Opacity;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 2], "%02d", value);
    }
    return 0;
}
