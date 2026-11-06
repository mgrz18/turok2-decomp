#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"


extern CPlayerOptions D_8011AB18; /* D_8011AAD8 + 0x40: default options */
extern char D_800B4080[];
extern char D_800B4098[];
extern char D_800C1318[]; /* "  %2d"-style */
extern char D_800C1320[];
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Label of the player's 0x80-centred setting at options+0x2A. */
s32 OptionsMenu_StickVertText(MenuItem *a, Menu *c) {
    CPlayerOptions *options = &D_8011AB18;
    s32 v;

    if (c->player != NULL && c->player->m_pOptions != NULL) {
        options = c->player->m_pOptions;
    }
    v = (options->m_SettingY - 0x80) / 8;
    if (v == 0) {
        a->text = D_800B4080;
    } else if (v > 0) {
        a->text = D_800B4098;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), D_800C1318, v);
    } else {
        a->text = D_800B4098;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), D_800C1320, v);
    }
    return 0;
}
