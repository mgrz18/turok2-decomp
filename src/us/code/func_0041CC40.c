#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItemTmpl;

typedef struct {
    /* 0x00 */ u16 count[4];
    /* 0x08 */ u8 pad8[0x38];
} SetupSlot; /* 0x40 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ void *player;
} MenuCaller;

extern u8 D_80119870[];        /* player list */
extern SetupSlot D_8011AB58[]; /* D_8011AAD8.slots */
extern char D_800C1330[];      /* number format */
extern s32 func_0025340C(void *players, void *player);
extern s32 func_004160F0(MenuItemTmpl *a);
extern void func_0029E3E0(char *dst, char *fmt, ...);

/* Prints the player's counter 0 into the label. */
s32 func_0041CC40(MenuItemTmpl *a, MenuCaller *c) {
    s32 n = D_8011AB58[func_0025340C(D_80119870, c->player)].count[0];

    func_0029E3E0(a->text + (func_004160F0(a) - 4), D_800C1330, n);
    return 0;
}
