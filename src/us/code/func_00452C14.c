#include "common.h"
#include "structs/game.h"

/* A menu item: its text is a format line whose last characters get the value. */
typedef struct MenuItem {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 type;
    /* 0x06 */ u8 pad6[0x14 - 6];
    /* 0x14 */ char *text;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ void *player;
    /* 0x20 */ void *options;
} MenuItem;

/* The multiplayer settings, D_8011AAD8 (0x184 bytes, see func_0042F514). */


s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B65D0[];
extern char D_800B65B8[];
extern char D_800B65A0[];
extern char D_800B65E8[];

/* Shows setting unk16 (0-255, centred on 0x80) in steps of 8 from -16 to 15. */
s32 func_00452C14(MenuItem *item) {
    s32 value = (D_8011AAD8.unk16 - 0x80) / 8;

    if (value == 0) {
        item->text = D_800B65D0;
    } else if (value == 15) {
        item->text = D_800B65B8;
    } else if (value == -16) {
        item->text = D_800B65A0;
    } else if (value > 0) {
        item->text = D_800B65E8;
        func_0029E3E0(&item->text[func_004160F0(item) - 4], "  %2d", value);
    } else {
        item->text = D_800B65E8;
        func_0029E3E0(&item->text[func_004160F0(item) - 4], " %3d", value);
    }
    return 0;
}
