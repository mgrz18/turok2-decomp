#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/game.h"

s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B4080[];
extern char D_800B4098[];

/* Shows the player's option unk2A (0-255, centred on 0x80) in steps of 8, falling back to the
 * default options when there is no player. */
s32 func_004555B0(MenuItem *item, Menu *menu) {
    CPlayerOptions *options = &D_8011AAD8.defaultSlot;
    s32 value;

    if (menu->player != NULL && menu->player->m_pOptions != NULL) {
        options = menu->player->m_pOptions;
    }
    value = (options->m_SettingY - 0x80) / 8;
    if (value == 0) {
        item->text = D_800B4080;
    } else if (value > 0) {
        item->text = D_800B4098;
        func_0029E3E0(&item->text[func_004160F0(item) - 4], "  %02d", value);
    } else {
        item->text = D_800B4098;
        func_0029E3E0(&item->text[func_004160F0(item) - 4], " %03d", value);
    }
    return 0;
}
