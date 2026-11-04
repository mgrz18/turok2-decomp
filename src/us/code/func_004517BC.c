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

extern char D_800B617C[];
extern char D_800B6164[];

/* Names setting 0x29. */
s32 func_004517BC(MenuItem *item) {
    switch (D_8011AAD8.tagUnk29) {
    case 0:
        item->text = D_800B617C;
        break;
    case 1:
        item->text = D_800B6164;
        break;
    }
    return 0;
}
