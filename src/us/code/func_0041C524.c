#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/game.h"


extern char D_800B4050[];
extern char D_800B4068[];
extern char D_800C1318[]; /* "  %2d"-style */
extern char D_800C1320[];
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Label of the player's 0x80-centred setting at options+0x29. */
s32 OptionsMenu_StickHorzText(MenuItem *a, Menu *c) {
    CPlayerOptions *options = &gEngineApp_m_Settings.defaultSlot;
    s32 v;

    if (c->player != NULL && c->player->m_pOptions != NULL) {
        options = c->player->m_pOptions;
    }
    v = (options->m_SettingX - 0x80) / 8;
    if (v == 0) {
        a->text = D_800B4050;
    } else if (v > 0) {
        a->text = D_800B4068;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), D_800C1318, v);
    } else {
        a->text = D_800B4068;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), D_800C1320, v);
    }
    return 0;
}
