/* WRONG CUT: the real function is func_00453380, size 0x2C: splat's func_00453380 starts with a stray
 * never-executed word (c.lt.s) ahead of the 16-aligned real start. */
#include "common.h"

typedef struct Layer {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ s32 flags;
} Layer;

typedef struct Screen {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ Layer *layer;
} Screen;

typedef struct MatchState {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 timer;
} MatchState;

extern MatchState D_8011ACB0;

void func_00453380(Screen *screen) {
    D_8011ACB0.timer = 120.0f;
    screen->layer->flags &= ~0x4000000;
}
