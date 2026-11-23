#include "common.h"
#include "globals/menu_text.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/game.h"


extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Label of the player's 0x80-centred setting at options+0x2A. */
s32 OptionsMenu_StickVertText(MenuItem *a, Menu *c) {
    CPlayerOptions *options = &gEngineApp_m_Settings.defaultSlot;
    s32 v;

    if (c->player != NULL && c->player->m_pOptions != NULL) {
        options = c->player->m_pOptions;
    }
    v = (options->m_SettingY - 0x80) / 8;
    if (v == 0) {
        a->text = sOptionsMenuText_ControlStickVertNorm;
    } else if (v > 0) {
        a->text = sOptionsMenuText_ControlStickVert;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sOptionsMenuFmt_02d, v);
    } else {
        a->text = sOptionsMenuText_ControlStickVert;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sOptionsMenuFmt_03d, v);
    }
    return 0;
}
