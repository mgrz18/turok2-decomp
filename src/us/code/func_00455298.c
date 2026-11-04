#include "common.h"
#include "structs/game.h"

/* A menu item: its text is a format line whose last characters get the value. */
typedef struct MenuItem {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ u32 flags;
    /* 0x0C */ u8 padC[0x14 - 0xC];
    /* 0x14 */ char *text;
} MenuItem;

typedef struct MenuPlayer {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ GameSlot *options;
} MenuPlayer;

typedef struct MenuContext {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ MenuPlayer *player;
} MenuContext;



s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B3FD8[];
extern char D_800B3FF0[];

s32 func_00455298(MenuItem *item) {
    if (D_8011AAD8.mode == 13 && D_8011AAD8.multiplayer == 0) {
        item->flags &= ~0x1000000;
    } else {
        item->flags |= 0x1000000;
    }
    switch (D_8011AAD8.unk1B) {
    case 0:
        item->text = D_800B3FD8;
        break;
    case 1:
        item->text = D_800B3FF0;
        break;
    }
    return 0;
}
