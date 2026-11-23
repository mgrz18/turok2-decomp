#include "common.h"
#include "globals/menu_text.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */


/* Shows the player's option unk2A (0-255, centred on 0x80) in steps of 8, falling back to the
 * default options when there is no player. */
s32 OptionsMenu_StickVertText_2(MenuItem *item, Menu *menu) {
    CPlayerOptions *options = &gEngineApp_m_Settings.defaultSlot;
    s32 value;

    if (menu->player != NULL && menu->player->m_pOptions != NULL) {
        options = menu->player->m_pOptions;
    }
    value = (options->m_SettingY - 0x80) / 8;
    if (value == 0) {
        item->text = sOptionsMenuText_ControlStickVertNorm;
    } else if (value > 0) {
        item->text = sOptionsMenuText_ControlStickVert;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", value);
    } else {
        item->text = sOptionsMenuText_ControlStickVert;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], " %03d", value);
    }
    return 0;
}
