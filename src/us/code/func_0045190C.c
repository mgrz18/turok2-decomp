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
