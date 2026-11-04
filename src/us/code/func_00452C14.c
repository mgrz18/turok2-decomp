#include "common.h"
#include "structs/menu.h"

/* The multiplayer settings, D_8011AAD8 (0x184 bytes, see func_0042F514). */
typedef struct MenuSettings {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 pad19[0x20 - 0x19];
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
