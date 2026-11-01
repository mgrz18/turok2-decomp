#include "common.h"

/* Player-name menu: highlights the row of the player the menu belongs to
 * and copies that player's name (trailing spaces trimmed, nothing when it is
 * still the default "player" name) into the edit buffer. */

typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 pad4[0x28 - 0x4];
} MenuRow;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ MenuRow rows[4];
} MenuWindow;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ MenuWindow *win;
    /* 0x10 */ u8 pad10[0x1C - 0x10];
    /* 0x1C */ void *player;
} Menu;

typedef struct {
    /* 0x00 */ s32 length;
    /* 0x04 */ s32 cursor;
    /* 0x08 */ s32 end;
    /* 0x0C */ char text[0x18 - 0xC];
} NameEdit;

extern u8 D_80119870[];
extern u8 D_8011AB8B[][0x40];   /* player options +0x33: names */
extern NameEdit D_800C1FB0[];
extern u8 D_800C1340[];   /* "player" */

extern s32 func_0025340C(void *players, void *player);

#define ROW_SELECTED 0x01800000

static __inline__ s32 nameCmp(u8 *a, u8 *b, s32 n) {
    u8 c1;
    u8 c2;

    do {
        c1 = *a++;
        c2 = *b++;
        n--;
        if (c1 != c2 || c1 == 0) {
            break;
        }
    } while (n == 0); /* sic: compares only the first character */
    return c1 - c2;
}

void func_0041D050(Menu *menu) {
    s32 index;
    s32 i;
    s32 spaces;
    u8 *name;
    u8 *dst;

    index = func_0025340C(D_80119870, menu->player);
    switch (index) {
    case 0:
    default:
        menu->win->rows[0].flags |= ROW_SELECTED;
        menu->win->rows[1].flags &= ~ROW_SELECTED;
        menu->win->rows[2].flags &= ~ROW_SELECTED;
        menu->win->rows[3].flags &= ~ROW_SELECTED;
        break;
    case 1:
        menu->win->rows[1].flags |= ROW_SELECTED;
        menu->win->rows[0].flags &= ~ROW_SELECTED;
        menu->win->rows[2].flags &= ~ROW_SELECTED;
        menu->win->rows[3].flags &= ~ROW_SELECTED;
        break;
    case 2:
        menu->win->rows[2].flags |= ROW_SELECTED;
        menu->win->rows[0].flags &= ~ROW_SELECTED;
        menu->win->rows[1].flags &= ~ROW_SELECTED;
        menu->win->rows[3].flags &= ~ROW_SELECTED;
        break;
    case 3:
        menu->win->rows[3].flags |= ROW_SELECTED;
        menu->win->rows[0].flags &= ~ROW_SELECTED;
        menu->win->rows[1].flags &= ~ROW_SELECTED;
        menu->win->rows[2].flags &= ~ROW_SELECTED;
        break;
    }
    D_800C1FB0[index].cursor = 0;
    dst = D_800C1FB0[index].text;
    name = D_8011AB8B[index];
    i = 8;
    spaces = 0;
    while (i > 0) {
        i--;
        if (name[i] != ' ') {
            break;
        }
        spaces++;
    }
    if (nameCmp(name, D_800C1340, 6) == 0) {
        spaces = 8;
    }
    for (i = 0; i < 8 - spaces; i++) {
        *dst++ = *name++;
    }
    *dst = 0;
    D_800C1FB0[index].length = 8 - spaces;
    D_800C1FB0[index].end = 8 - spaces;
}
