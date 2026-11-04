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

s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B608C[];
extern char D_800B60A4[];
extern char D_800B60BC[];
extern char D_800B60D4[];
extern char D_800B60EC[];
extern char D_800B6104[];
extern char D_800B611C[];
extern char D_800B6134[];
extern char D_800B614C[];

/* Names setting 0x28. */
s32 func_004516B0(MenuItem *item) {
    switch (D_8011AAD8.tagSkill) {
    case 0:
        item->text = D_800B608C;
        break;
    case 1:
        item->text = D_800B60A4;
        break;
    case 2:
        item->text = D_800B60BC;
        break;
    case 3:
        item->text = D_800B60D4;
        break;
    case 4:
        item->text = D_800B60EC;
        break;
    case 5:
        item->text = D_800B6104;
        break;
    case 6:
        item->text = D_800B611C;
        break;
    case 7:
        item->text = D_800B6134;
        break;
    case 8:
        item->text = D_800B614C;
        break;
    }
    return 0;
}
