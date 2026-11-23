#include "common.h"
#include "globals/menu_text.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/cache.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */



/* Shows setting 0x0C in sixteenths: a fixed line for 0, otherwise the number. */
s32 OptionsMenu_BrightnessText_2(MenuItem *item) {
    u32 value = gEngineApp_m_Settings.unkC >> 4;

    CCache__SetFrameDelay(CCache__GetFrameDelay() + 2);
    if (value == 0) {
        item->text = sOptionsMenuText_BrightnessNorm;
    } else {
        item->text = sOptionsMenuText_Brightness;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", value);
    }
    return 0;
}
