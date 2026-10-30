/* NON-MATCHING: 10 of 199 words differ: leaveMenu's two byte stores go through 'lui $at' instead of a base register ($v1 = &D_8011AAD8, and $a1 - 0x1B0 after the mode check); a do{}while(0) around them gets the base register but schedules the 'a0 = menu' copy after the stores. */
#include "common.h"

typedef struct MenuContext {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s32 controller;
} MenuContext;

/* What this reads around the multiplayer settings, D_8011AAD8. */
typedef struct MenuSettings {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ u32 flags;
    /* 0x008 */ u8 pad8[0x19 - 8];
    /* 0x019 */ u8 unk19;
    /* 0x01A */ u8 unk1A;
    /* 0x01B */ u8 pad1B[0x1B0 - 0x1B];
    /* 0x1B0 */ s32 mode;
} MenuSettings;

extern MenuSettings D_8011AAD8;
extern u32 D_8011AADC; /* D_8011AAD8.flags */
extern s32 D_80130924; /* the selected option, -1 for none */
extern char D_800F6CB0[];
extern char D_800F7078[];

s32 func_002855E8(s32 controller);
s32 func_00285628(s32 controller);
s32 func_00285670(s32 controller);
void func_00417ACC(MenuContext *menu);
void func_00288C5C(void *, s32);
void func_002886D0(void *, s32);
void func_0042C620(void *, s32, s32);

/* The settings flag that enables option `index` (see func_00453430). */
static __inline__ u32 optionBit(s32 index) {
    u32 bit;

    switch (index) {
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

/* The next enabled option from the selected one, stepping by `step` and wrapping; -1 if none. */
static __inline__ s32 findOption(s32 step) {
    s32 index = D_80130924;
    s32 n;

    for (n = 0; n < 11; n++) {
        index += step;
        if (index > 10) {
            index = 0;
        } else if (index < 0) {
            index = 10;
        }
        if (D_8011AADC & optionBit(index)) {
            return index;
        }
    }
    return -1;
}

static __inline__ void leaveMenu(MenuContext *menu) {
    MenuSettings *s = &D_8011AAD8;

    s->unk19 = 0;
    s->unk1A = 1;
    func_00417ACC(menu);
}

/* The option list: A starts the chosen option's level, left/right step to the previous/next enabled one. */
s32 func_0045353C(void *item, MenuContext *menu) {
    s32 level;

    switch (D_80130924) {
    case 0:
    default:
        level = 1000;
        break;
    case 1:
        level = 2000;
        break;
    case 2:
        level = 3000;
        break;
    case 3:
        level = 4000;
        break;
    case 4:
        level = 5000;
        break;
    case 5:
        level = 6000;
        break;
    case 6:
        level = 7401;
        break;
    case 7:
        level = 7501;
        break;
    case 8:
        level = 7601;
        break;
    case 9:
        level = 7602;
        break;
    case 10:
        level = 8000;
        break;
    }
    if (func_002855E8(menu->controller)) {
        if (level == 8000) {
            leaveMenu(menu);
            func_00288C5C(D_800F6CB0, 16);
            return 1;
        }
        if (D_8011AAD8.mode == 13) {
            func_0042C620(D_800F7078, level, 2);
            return 0;
        }
        leaveMenu(menu);
        func_002886D0(D_800F6CB0, level);
        return 1;
    }
    if (func_00285628(menu->controller)) {
        D_80130924 = findOption(-1);
        return 0;
    }
    if (func_00285670(menu->controller)) {
        D_80130924 = findOption(1);
        return 0;
    }
    return 0;
}
