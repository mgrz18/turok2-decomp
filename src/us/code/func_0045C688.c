/* WRONG CUT: the real function starts at 0x0045C688 (size 0x118 = 280): splat
 * cut its first two words (lui/lw D_80130990) as func_0045C688. */
#include "common.h"

/* Start the selected level, then open the next menu page (on the player's own
 * menu when there is a player). */

typedef struct {
    u8 pad0[4];
    s8 mode; /* 0x04 */
} MenuArgs;

typedef struct {
    u8 pad0[0x1C];
    u8 *player;     /* 0x1C */
    MenuArgs *args; /* 0x20 */
    void *page;     /* 0x24 */
} MenuEvent;

extern s32 D_80130990;
extern s32 D_800C2028;
extern s32 D_801309BC;
extern s32 D_80130940;
extern u8 D_80119E2C[];
extern u8 D_0043BE14[];
extern u8 D_0043BDF0[];

s32 func_0041DCB0(MenuEvent *event, s32 mode, s32 arg2);
void func_00416644(void *menu, void *page, void *player, void *args, s32 flags);

static __inline__ s32 currentMode(MenuEvent *event) {
    s32 mode;

    if (D_80130990 != 0) {
        mode = D_800C2028;
    } else {
        mode = event->args->mode;
    }
    return mode;
}

s32 func_0045C688(void *arg0, MenuEvent *event) {
    s32 mode = currentMode(event);
    void *menu;

    D_801309BC = 1;
    if (func_0041DCB0(event, mode, 1) != 0) {
        if (event->player != NULL) {
            menu = *(u8 **)(event->player + 0x51C) + 0x53C;
        } else {
            menu = D_80119E2C;
        }
        if (event->page == D_0043BE14 || event->page == D_0043BDF0) {
            D_80130940 = 0;
        }
        func_00416644(menu, event->page, event->player, event->args, 0);
        return 1;
    } else {
        if (event->page == D_0043BE14 || event->page == D_0043BDF0) {
            D_80130940 = 0;
        }
        func_00416644(D_80119E2C, event->page, event->player, event->args, 0);
        return 1;
    }
}
