#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"


extern CPlayerOptions D_8011AB18; /* D_8011AAD8 + 0x40: default options */
extern char D_800B4050[];
extern char D_800B4068[];
extern char D_800C1318[]; /* "  %2d"-style */
extern char D_800C1320[];
extern s32 func_004160F0(MenuItem *a);
extern void func_0029E3E0(char *dst, char *fmt, ...);

/* Label of the player's 0x80-centred setting at options+0x29. */
s32 func_0041C524(MenuItem *a, Menu *c) {
    CPlayerOptions *options = &D_8011AB18;
    s32 v;

    if (c->player != NULL && c->player->m_pOptions != NULL) {
        options = c->player->m_pOptions;
    }
    v = (options->m_SettingX - 0x80) / 8;
    if (v == 0) {
        a->text = D_800B4050;
    } else if (v > 0) {
        a->text = D_800B4068;
        func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C1318, v);
    } else {
        a->text = D_800B4068;
        func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C1320, v);
    }
    return 0;
}
