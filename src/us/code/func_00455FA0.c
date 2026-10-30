#include "common.h"

/* Name entry: highlight the current player's column and load the player's
 * name (trailing spaces trimmed; the default name D_800C1340 counts as empty)
 * into the editing buffer. */

typedef struct {
    s32 cursor;    /* 0x00 */
    f32 timer;     /* 0x04 */
    s32 length;    /* 0x08 */
    char name[12]; /* 0x0C */
} NameEntry; /* 0x18 bytes */

typedef struct {
    u8 pad0[8];
    u32 flags; /* 0x08 */
    u8 padC[0x1C];
} MenuEntry; /* 0x28 bytes */

typedef struct {
    u8 pad0[0x28];
    MenuEntry entries[4]; /* 0x28 */
} Menu;

typedef struct {
    u8 pad0[0xC];
    Menu *menu;   /* 0x0C */
    u8 pad10[0xC];
    void *player; /* 0x1C */
} MenuEvent;

#define ENTRY_SELECTED 0x01800000

extern u8 D_80119870[];
extern NameEntry D_800C1FB0[];
extern u8 D_8011AB8B[];
extern u8 D_800C1340[];

s32 func_0025340C(void *state, void *player);

static __inline__ s32 nameEquals(u8 *a, u8 *b, s32 count) {
    u8 c;
    u8 d;
    do {
        c = *a++;
        d = *b++;
        count--;
    } while (c == d && c != 0 && count == 0);
    return c == d;
}

void func_00455FA0(MenuEvent *event) {
    s32 i = func_0025340C(D_80119870, event->player);
    s32 trailing;
    char *dst;
    u8 *src;
    s32 n;
    s32 k;

    switch (i) {
    case 0:
    default:
        event->menu->entries[0].flags |= ENTRY_SELECTED;
        event->menu->entries[1].flags &= ~ENTRY_SELECTED;
        event->menu->entries[2].flags &= ~ENTRY_SELECTED;
        event->menu->entries[3].flags &= ~ENTRY_SELECTED;
        break;
    case 1:
        event->menu->entries[1].flags |= ENTRY_SELECTED;
        event->menu->entries[0].flags &= ~ENTRY_SELECTED;
        event->menu->entries[2].flags &= ~ENTRY_SELECTED;
        event->menu->entries[3].flags &= ~ENTRY_SELECTED;
        break;
    case 2:
        event->menu->entries[2].flags |= ENTRY_SELECTED;
        event->menu->entries[0].flags &= ~ENTRY_SELECTED;
        event->menu->entries[1].flags &= ~ENTRY_SELECTED;
        event->menu->entries[3].flags &= ~ENTRY_SELECTED;
        break;
    case 3:
        event->menu->entries[3].flags |= ENTRY_SELECTED;
        event->menu->entries[0].flags &= ~ENTRY_SELECTED;
        event->menu->entries[1].flags &= ~ENTRY_SELECTED;
        event->menu->entries[2].flags &= ~ENTRY_SELECTED;
        break;
    }

    D_800C1FB0[i].timer = 0.0f;
    dst = D_800C1FB0[i].name;
    src = &D_8011AB8B[i * 64];
    for (n = 8, trailing = 0; n > 0; trailing++) {
        if (src[--n] != ' ') {
            break;
        }
    }
    if (nameEquals(src, D_800C1340, 6)) {
        trailing = 8;
    }
    for (k = 0; k < 8 - trailing; k++) {
        *dst++ = *src++;
    }
    *dst = '\0';
    D_800C1FB0[i].cursor = 8 - trailing;
    D_800C1FB0[i].length = 8 - trailing;
}
