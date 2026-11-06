#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B4050[];
extern char D_800B4068[];

/* Shows the player's option unk29 (0-255, centred on 0x80) in steps of 8, falling back to the
 * default options when there is no player. */
s32 OptionsMenu_StickHorzText_2(MenuItem *item, Menu *menu) {
    CPlayerOptions *options = &gEngineApp_m_Settings.defaultSlot;
    s32 value;

    if (menu->player != NULL && menu->player->m_pOptions != NULL) {
        options = menu->player->m_pOptions;
    }
    value = (options->m_SettingX - 0x80) / 8;
    if (value == 0) {
        item->text = D_800B4050;
    } else if (value > 0) {
        item->text = D_800B4068;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", value);
    } else {
        item->text = D_800B4068;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], " %03d", value);
    }
    return 0;
}
