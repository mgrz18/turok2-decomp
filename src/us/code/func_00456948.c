/* WRONG CUT: the real function starts at 0x00456948 (size 0x234 = 564): splat
 * gave its first two words (lui/lw D_80130990) to the function before. */
#include "common.h"

/* Level select: grey out the entries (16) that cannot be chosen in the current
 * game mode. */

typedef struct {
    u8 pad0[8];
    u32 flags; /* 0x08 */
    u8 padC[0x1C];
} MenuEntry; /* 0x28 bytes */

typedef struct {
    u8 pad0[0x78];
    MenuEntry entries[16]; /* 0x78 */
} Menu;

typedef struct {
    u8 pad0[4];
    s8 mode; /* 0x04 */
} MenuArgs;

typedef struct {
    u8 pad0[0xC];
    Menu *menu;     /* 0x0C */
    u8 pad10[0x10];
    MenuArgs *args; /* 0x20 */
} MenuEvent;

#define ENTRY_DISABLED 0x01000000

extern s32 D_80130990;
extern s32 D_800C2028;
extern s32 D_800C2024;
extern char D_800B45D8[];
extern char D_800B45D4[];
extern char D_800B45C0[];
extern char D_800B45CC[];

s32 func_00425D94(s32 mode, s32 index, s32 *valid, char *name, void *arg4, s32 *arg5, char *arg6, char *arg7);
s32 func_00426CAC(char *s);
s32 func_00426C74(char *a, char *b);

static __inline__ s32 isSpecial(char *a, char *b) {
    if (func_00426CAC(b) == 4 && func_00426CAC(a) == 2 && func_00426C74(b, D_800B45D8) == 0 && func_00426C74(a, D_800B45D4) == 0) {
        return 1;
    }
    return 0;
}

static __inline__ void enableEntry(MenuEntry *entry) {
    entry->flags &= ~ENTRY_DISABLED;
}

static __inline__ void disableEntry(MenuEntry *entry) {
    entry->flags |= ENTRY_DISABLED;
}

void func_00456948(MenuEvent *event) {
    s32 mode;
    s32 i;
    s32 enabled;
    s32 failed;
    s32 special;
    u8 sp20[8];
    char a[8];
    char b[8];
    char name[16];
    s32 valid;
    s32 sp4C;

    if (D_80130990 != 0) {
        mode = D_800C2028;
    } else {
        mode = event->args->mode;
    }
    for (i = 0; i < 16; i++) {
        failed = func_00425D94(mode, i, &valid, name, sp20, &sp4C, a, b);
        enabled = 1;
        if (failed == 0 && valid != 0) {
            switch (D_800C2024) {
            case 1:
                special = isSpecial(a, b);
                if (func_00426C74(name, D_800B45C0) == 0 && special) {
                    enabled = 0;
                }
                break;
            case 2:
                special = isSpecial(a, b);
                if (func_00426C74(name, D_800B45CC) == 0 && special) {
                    enabled = 0;
                }
                break;
            case 0:
            default:
                enabled = 0;
                break;
            }
        }
        if (enabled) {
            enableEntry(&event->menu->entries[i]);
        } else {
            disableEntry(&event->menu->entries[i]);
        }
    }
}
