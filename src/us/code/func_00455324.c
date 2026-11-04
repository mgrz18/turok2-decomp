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

extern char D_800B4008[];
extern char D_800B4020[];
extern char D_800B4038[];

/* Shows setting 0x10 in sixteenths: two fixed lines for 0 and 16, otherwise the number. */
s32 func_00455324(MenuItem *item) {
    s32 value = D_8011AAD8.unk10 >> 4;

    if (value == 0) {
        item->text = D_800B4008;
    } else if (value == 16) {
        item->text = D_800B4020;
    } else {
        item->text = D_800B4038;
        func_0029E3E0(&item->text[func_004160F0(item) - 2], "%02d", value);
    }
    return 0;
}
