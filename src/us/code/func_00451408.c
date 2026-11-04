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
} MenuSettings;

extern MenuSettings D_8011AAD8;

s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

/* Writes setting unk25 (or "none" when 0) over the end of the item's text. */
s32 func_00451408(MenuItem *item) {
    MenuSettings *s = &D_8011AAD8;

    if (s->unk25 != 0) {
        func_0029E3E0(&item->text[func_004160F0(item) - 4], "  %02d", s->unk25);
    } else {
        func_0029E3E0(&item->text[func_004160F0(item) - 4], "none");
    }
    return 0;
}
