/* Module-2 version of func_0041A5DC (the warp menu); here both exits also clear the multiplayer flag. */
#include "common.h"
#include "structs/menu.h"

/* Level select (warp) menu input: A warps to the selected level (or, on the
 * last entry, starts the credits/next mode), up and down move the selection
 * to the previous/next level whose key the player owns. */

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ u32 keys;
    /* 0x08 */ u8 pad8[0x19 - 0x8];
    /* 0x19 */ u8 multiplayer;
    /* 0x1A */ u8 warping;
} Progress;

typedef struct {
    /* 0x0000 */ u8 pad0[0x1268];
    /* 0x1268 */ Progress progress;
    /* 0x1284 */ u8 pad1284[0x1418 - 0x1284];
    /* 0x1418 */ s32 mode;
} Game;

extern Game D_80119870;
#define PROGRESS (&D_80119870.progress)
extern s32 D_80130924;
extern u8 D_800F6CB0[];
extern u8 D_800F7078[];

extern s32 func_002855E8(s32 pad);
extern s32 func_00285628(s32 pad);
extern s32 func_00285670(s32 pad);
extern void func_00417ACC(Menu *c);
extern void func_00288C5C(void *game, s32 mode);
extern void func_002886D0(void *game, s32 level);
extern void func_0042C620(void *level, s32 id, s32 how);

static __inline__ u32 levelKey(s32 level) {
    u32 bit;

    switch (level) {
    case 0:
    default:
        bit = 0x10000;
        break;
    case 1:
        bit = 0x20000;
        break;
    case 2:
        bit = 0x40000;
        break;
    case 3:
        bit = 0x80000;
        break;
    case 4:
        bit = 0x100000;
        break;
    case 5:
        bit = 0x200000;
        break;
    case 6:
        bit = 0x800000;
        break;
    case 7:
        bit = 0x400000;
        break;
    case 8:
        bit = 0x1000000;
        break;
    case 9:
        bit = 0x2000000;
        break;
    case 10:
        bit = 0x4000000;
        break;
    }
    return bit;
}

/* The next level from the selection in direction step whose key is owned, or -1. */
static __inline__ s32 findLevel(s32 step) {
    s32 sel;
    s32 n;
    u32 keys;

    sel = D_80130924;
    keys = D_80119870.progress.keys;
    for (n = 0; n < 11; n++) {
        sel += step;
        if (sel > 10) {
            sel = 0;
        } else if (sel < 0) {
            sel = 10;
        }
        if (keys & levelKey(sel)) {
            return sel;
        }
    }
    return -1;
}

s32 func_0045353C(void *menu, Menu *c) {
    s32 id;

    switch (D_80130924) {
    case 0:
    default:
        id = 1000;
        break;
    case 1:
        id = 2000;
        break;
    case 2:
        id = 3000;
        break;
    case 3:
        id = 4000;
        break;
    case 4:
        id = 5000;
        break;
    case 5:
        id = 6000;
        break;
    case 6:
        id = 0x1CE9;
        break;
    case 7:
        id = 0x1D4D;
        break;
    case 8:
        id = 0x1DB1;
        break;
    case 9:
        id = 0x1DB2;
        break;
    case 10:
        id = 8000;
        break;
    }
    if (func_002855E8(c->controller) != 0) {
        if (id == 8000) {
            PROGRESS->multiplayer = 0;
            PROGRESS->warping = 1;
            func_00417ACC(c);
            func_00288C5C(D_800F6CB0, 16);
            return 1;
        }
        if (D_80119870.mode == 13) {
            func_0042C620(D_800F7078, id, 2);
            return 0;
        }
        PROGRESS->multiplayer = 0;
        PROGRESS->warping = 1;
        func_00417ACC(c);
        func_002886D0(D_800F6CB0, id);
        return 1;
    }
    if (func_00285628(c->controller) != 0) {
        D_80130924 = findLevel(-1);
    } else if (func_00285670(c->controller) != 0) {
        D_80130924 = findLevel(1);
    } else {
        return 0;
    }
    return 0;
}
