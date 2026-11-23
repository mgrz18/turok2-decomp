#include "common.h"
#include "globals/menu_text.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */


s32 OptionsMenu_TwoPlayerLayoutText_2(MenuItem *item) {
    if (gEngineApp_m_Settings.mode == 13 && gEngineApp_m_Settings.multiplayer == 0) {
        item->flags &= ~0x1000000;
    } else {
        item->flags |= 0x1000000;
    }
    switch (gEngineApp_m_Settings.unk1B) {
    case 0:
        item->text = sOptionsMenuText_2PlyrLayoutHorz;
        break;
    case 1:
        item->text = sOptionsMenuText_2PlyrLayoutVert;
        break;
    }
    return 0;
}
