#include "common.h"
#include "structs/menu.h"

/* The multiplayer settings, D_8011AAD8 (0x184 bytes, see func_0042F514). */
typedef struct MenuSettings {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s8 unk20;
    /* 0x21 */ s8 unk21;
    /* 0x22 */ s8 unk22;
    /* 0x23 */ s8 unk23;
    /* 0x24 */ s8 unk24;
    /* 0x25 */ s8 unk25;
    /* 0x26 */ s8 unk26;
    /* 0x27 */ s8 unk27;
    /* 0x28 */ s8 unk28;
    /* 0x29 */ u8 unk29;
    /* 0x2A */ s8 unk2A;
    /* 0x2B */ s8 unk2B;
} MenuSettings;

extern MenuSettings D_8011AAD8;

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
    switch (D_8011AAD8.unk27) {
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
