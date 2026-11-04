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

extern char D_800B5F3C[];
extern char D_800B61D8[];
extern char D_800B61F0[];
extern char D_800B6208[];
extern char D_800B6220[];
extern char D_800B6238[];
extern char D_800B6250[];

/* Names setting 0x27. */
s32 func_004515C0(MenuItem *item) {
    switch (D_8011AAD8.tagArena) {
    case 0:
        item->text = D_800B5F3C;
        break;
    case 1:
        item->text = D_800B61D8;
        break;
    case 2:
        item->text = D_800B61F0;
        break;
    case 3:
        item->text = D_800B6208;
        break;
    case 4:
        item->text = D_800B6220;
        break;
    case 5:
        item->text = D_800B6238;
        break;
    case 6:
        item->text = D_800B6250;
        break;
    }
    return 0;
}
