/* WRONG CUT: the real function is GameOverMenu_Init_2, size 0x2C: splat's GameOverMenu_Init_2 starts with a stray
 * never-executed word (c.lt.s) ahead of the 16-aligned real start. */
#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

typedef struct Layer {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ s32 flags;
} Layer;

typedef struct Screen {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ Layer *layer;
} Screen;

void GameOverMenu_Init_2(Screen *screen) {
    gEngineApp_m_Settings.match.countdown = 120.0f;
    screen->layer->flags &= ~0x4000000;
}
