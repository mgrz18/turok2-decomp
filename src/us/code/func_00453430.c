/* WRONG CUT: the real function is CheatsMenu_Init_2, size 0x10C: splat's CheatsMenu_Init_2 starts with a stray
 * never-executed word (a jal) ahead of the 16-aligned real start. */
#include "common.h"
#include "globals/menu.h"

typedef struct Layer {
    /* 0x000 */ u8 pad0[0x2D8];
    /* 0x2D8 */ s32 flags;
} Layer;

typedef struct Screen {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ Layer *layer;
} Screen;

extern u32 gEngineApp_m_Settings_unlockedCheats; /* local type: GameSettings.unlockedCheats; without this declaration here (globals/game.h alone) GCC loads the address instead of the value in the loop */

/* Picks the first of the 11 options whose flag is set in the settings (-1 if none). */
void CheatsMenu_Init_2(Screen *screen) {
    s32 index = 0;
    s32 n;
    u32 bit;

    for (n = 0; n < 11; n++) {
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
        if (gEngineApp_m_Settings_unlockedCheats & bit) {
            goto found;
        }
        index++;
        if (index > 10) {
            index = 0;
        } else if (index < 0) {
            index = 10;
        }
    }
    index = -1;
found:
    sCheatsMenuSelection = index;
    if (index != -1) {
        screen->layer->flags |= 0x1800000;
    } else {
        screen->layer->flags &= ~0x1800000;
    }
}
