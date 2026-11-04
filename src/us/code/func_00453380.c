/* WRONG CUT: the real function is func_00453380, size 0x2C: splat's func_00453380 starts with a stray
 * never-executed word (c.lt.s) ahead of the 16-aligned real start. */
#include "common.h"
#include "structs/game.h"

typedef struct Layer {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ s32 flags;
} Layer;

typedef struct Screen {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ Layer *layer;
} Screen;


void func_00453380(Screen *screen) {
    D_8011ACB0.countdown = 120.0f;
    screen->layer->flags &= ~0x4000000;
}
