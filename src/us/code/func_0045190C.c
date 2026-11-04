#include "common.h"
#include "structs/menu.h"

/* The multiplayer settings, D_8011AAD8 (0x184 bytes, see func_0042F514). */
typedef struct MenuSettings {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 speed;
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

extern s32 D_800C1EA8;     /* the speed setting, 0-15 (5 is normal) */
extern char D_800B6698[];
extern char D_800B66B0[];
extern char D_800C0E9C[];  /* "%3d", earlier in the file */

/* Names the game speed: a fixed line at 1.0, otherwise the setting relative to 5. */
s32 func_0045190C(MenuItem *item) {
    if (D_8011AAD8.speed == 1.0f) {
        item->text = D_800B66B0;
    } else {
        item->text = D_800B6698;
        func_0029E3E0(&item->text[func_004160F0(item) - 3], D_800C0E9C, D_800C1EA8 - 5);
    }
    return 0;
}
