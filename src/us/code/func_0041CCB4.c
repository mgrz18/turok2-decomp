#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"


typedef struct {
    /* 0x00 */ u8 pad0[0x33];
    /* 0x33 */ char name[8];
    /* 0x3B */ u8 pad3B[5];
} SetupSlot; /* 0x40 bytes */

extern s32 D_8011ACE4;         /* match started */
extern SetupSlot D_8011AB58[]; /* D_8011AAD8.slots */
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
    src = D_8011AB58[slot].name;
    for (i = 0; i < 8; i++) {
        *dst++ = *src++;
    }
    func_0029E3E0(a->text + (func_004160F0(a) - 2), D_800C1338, n);
    return 0;
}
