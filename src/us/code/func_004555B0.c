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

extern char D_800B4080[];
extern char D_800B4098[];

/* Shows the player's option unk2A (0-255, centred on 0x80) in steps of 8, falling back to the
 * default options when there is no player. */
s32 func_004555B0(MenuItem *item, MenuContext *menu) {
    GameSlot *options = &D_8011AAD8.defaultSlot;
    s32 value;

    if (menu->player != NULL && menu->player->options != NULL) {
        options = menu->player->options;
    }
    value = (options->unk2A - 0x80) / 8;
    if (value == 0) {
        item->text = D_800B4080;
    } else if (value > 0) {
        item->text = D_800B4098;
        func_0029E3E0(&item->text[func_004160F0(item) - 4], "  %02d", value);
    } else {
        item->text = D_800B4098;
        func_0029E3E0(&item->text[func_004160F0(item) - 4], " %03d", value);
    }
    return 0;
}
