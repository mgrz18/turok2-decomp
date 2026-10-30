/* WRONG CUT: the real function starts at 0x004595D8 (size 0x1A4 = 420): splat
 * cut its first two words (lui/lw D_80130990) as func_004595D8. */
#include "common.h"

/* Level chosen: load it, or set up the new player from the saved settings, or
 * open the next menu page. */

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

typedef struct {
    u16 counts[4]; /* 0x00 */
    u8 pad8[0x28];
    u8 team;       /* 0x30 */
    u8 pad31[2];
    u8 name[8];    /* 0x33 */
} PlayerOptions;

extern s32 D_80130990;
extern s32 D_800C2028;
extern s32 D_801309BC;
extern s32 D_80130960;
extern s32 D_801309C4;
extern u8 D_80130970;
extern u16 D_80130968[];
extern u16 D_8013096A[];
extern u16 D_8013096C[];
extern u16 D_8013096E[];
extern u8 D_80130974[8];
extern s8 D_80130950;
extern u8 D_80119870[];
extern u8 D_80119E2C[];
extern u8 D_00438668[];
extern u8 D_0043BCD0[];
extern u8 D_0043BD3C[];

s32 func_0041DCB0(MenuEvent *event, s32 mode, s32 arg2);
u8 *func_0025342C(void *state, MenuArgs *args);
void func_0042E350(u8 *player, s32 arg1);
void func_00416894(void *menu, MenuEvent *event, void *page);
s32 func_00425F84(s32 mode, s32 arg1);
void func_00416644(void *menu, void *page, void *player, void *args, void *prev);

static __inline__ s32 currentMode(MenuEvent *event) {
    s32 mode;

    if (D_80130990 != 0) {
        mode = D_800C2028;
    } else {
        mode = event->args->mode;
    }
    return mode;
}

s32 func_004595D8(void *arg0, MenuEvent *event) {
    s32 mode = currentMode(event);
    u8 *player;
    PlayerOptions *options;
    s32 i;

    if (func_0041DCB0(event, mode, 0) != 0) {
        D_801309BC = 1;
        return 1;
    }
    if (D_80130960 != 0) {
        player = func_0025342C(D_80119870, event->args);
        event->player = player;
        options = *(PlayerOptions **)(player + 0x518);
        options->team = D_80130970;
        options->counts[0] = D_80130968[0];
        options->counts[1] = D_8013096A[0];
        options->counts[2] = D_8013096C[0];
        options->counts[3] = D_8013096E[0];
        for (i = 0; i < 8; i++) {
            options->name[i] = D_80130974[i];
        }
        func_0042E350(player, D_80130950);
        func_00416894(*(u8 **)(player + 0x51C) + 0x53C, event, D_00438668);
        return 1;
    }
    if (func_00425F84(mode, D_801309C4) == 0) {
        func_00416644(D_80119E2C, D_0043BCD0, event->player, event->args, event->page);
    } else {
        func_00416644(D_80119E2C, D_0043BD3C, event->player, event->args, event->page);
    }
    return 1;
}
