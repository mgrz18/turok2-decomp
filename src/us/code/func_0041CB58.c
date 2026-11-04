#include "common.h"
#include "structs/menu.h"
#include "structs/player_options.h"

extern u8 D_80119870[];        /* player list */
extern CPlayerOptions D_8011AB58[]; /* D_8011AAD8.slots */
extern char D_800C1330[];      /* number format */
extern s32 func_0025340C(void *players, void *player);
extern s32 func_004160F0(MenuItem *a);
extern void func_0029E3E0(char *dst, char *fmt, ...);

/* Prints the player's kills count into the label. */
s32 func_0041CB58(MenuItem *a, Menu *c) {
    s32 n = D_8011AB58[func_0025340C(D_80119870, c->player)].m_Kills;

    func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C1330, n);
    return 0;
}
