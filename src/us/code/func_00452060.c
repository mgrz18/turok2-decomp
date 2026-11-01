#include "common.h"

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
typedef struct MenuSettings {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s8 unk20;
    /* 0x21 */ s8 unk21;
    /* 0x22 */ s8 unk22;
    /* 0x23 */ s8 unk23;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ s8 unk25;
    /* 0x26 */ s8 unk26;
    /* 0x27 */ s8 unk27;
    /* 0x28 */ s8 unk28;
    /* 0x29 */ u8 unk29;
    /* 0x2A */ s8 unk2A;
    /* 0x2B */ s8 unk2B;
    /* 0x2C */ s8 unk2C;
    /* 0x2D */ s8 unk2D;
    /* 0x2E */ u8 unk2E;
} MenuSettings;

extern MenuSettings D_8011AAD8;

s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B5F3C[];
extern char D_800B5F54[];
extern char D_800B5F6C[];
extern char D_800B5F84[];
extern char D_800B5F9C[];
extern char D_800B5FB4[];
extern char D_800B5FCC[];
extern char D_800B5FE4[];
extern char D_800B5FFC[];
extern char D_800B6014[];
extern char D_800B602C[];
extern char D_800B6044[];
extern char D_800B605C[];
extern char D_800B6074[];

/* Names setting 0x2C. */
s32 func_00452060(MenuItem *item) {
    switch (D_8011AAD8.unk2C) {
    case 0:
        item->text = D_800B5F3C;
        break;
    case 1:
        item->text = D_800B5F54;
        break;
    case 2:
        item->text = D_800B5F6C;
        break;
    case 3:
        item->text = D_800B5F84;
        break;
    case 4:
        item->text = D_800B5F9C;
        break;
    case 5:
        item->text = D_800B5FB4;
        break;
    case 6:
        item->text = D_800B5FCC;
        break;
    case 7:
        item->text = D_800B5FE4;
        break;
    case 8:
        item->text = D_800B5FFC;
        break;
    case 9:
        item->text = D_800B6014;
        break;
    case 10:
        item->text = D_800B602C;
        break;
    case 11:
        item->text = D_800B6044;
        break;
    case 12:
        item->text = D_800B605C;
        break;
    case 13:
        item->text = D_800B6074;
        break;
    }
    return 0;
}
