#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/player_options.h"

extern s32 D_8011ACE4;         /* match started */
extern CPlayerOptions D_8011AB58[]; /* D_8011AAD8.slots */
extern char D_800C1338[];      /* number format */
extern s32 func_004160F0(MenuItem *a);
extern void func_0029E3E0(char *dst, char *fmt, ...);

/* "<name> <count>" line of the score table, for the slot of item index - 2. */
s32 func_0041CCB4(MenuItem *a, Menu *c) {
    s32 slot = a->index - 2;
    CPlayer *player = c->player;
    s32 n;
    char *src;
    char *dst;
    s32 i;

    if (D_8011ACE4) {
        n = player->m_pOptions->m_Tagged[slot];
    } else {
        n = player->m_pOptions->m_Killed[slot];
    }
    dst = a->text;
    src = (char *)D_8011AB58[slot].m_Name;
    for (i = 0; i < 8; i++) {
        *dst++ = *src++;
    }
    func_0029E3E0(a->text + (func_004160F0(a) - 2), D_800C1338, n);
    return 0;
}
