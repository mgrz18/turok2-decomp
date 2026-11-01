/* NON-MATCHING: 7 of 42 words differ (W batch). */
/* MATCHES at its real extent (checked with the symbol size patched to 0xA8); the do/while(0) blocks give the register choice and store order (as in func_00430454, its first-module copy). */
/* WRONG CUT: the real function is 0x00469374-0x0046941C (size 0xA8 = 168):
 * splat cut it at the data label D_00469400. */
#include "common.h"

/* Start a level: check that the warp target exists, reset the multiplayer
 * options and switch the game state. */

typedef struct {
    u8 pad0[0x23FDC];
    s32 nextState;  /* 0x23FDC */
    u8 pad23FE0;
    u8 mode;        /* 0x23FE1 */
    u8 pad23FE2[0x16];
    s32 warpId;     /* 0x23FF8 */
    s32 warpFlags;  /* 0x23FFC */
} Game;

typedef struct {
    s32 values[4]; /* 0x00 */
    u8 pad10[0x34];
    s32 value44;   /* 0x44 */
} Options;

extern u8 D_800F7078[];
extern Options D_8011ACB0;

s32 func_0042C0B4(void *level, s32 a, s32 id, void *out, s32 flags);

void func_00469374(Game *game, s32 warpId) {
    u8 sp18[0x18];
    s32 id;

    id = warpId;

    if (func_0042C0B4(D_800F7078, -1, id, sp18, 1) == 0) {
        id = 0;
    }
    D_8011ACB0.values[0] = 0;
    D_8011ACB0.values[1] = 0;
    D_8011ACB0.values[2] = 0;
    D_8011ACB0.values[3] = 0;
    D_8011ACB0.value44 = 0;
    do {
        game->warpFlags = 0;
        game->warpId = id;
    } while (0);
    do {
        game->mode = 2;
        game->nextState = 13;
    } while (0);
}
