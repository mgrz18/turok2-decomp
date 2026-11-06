/* MATCHES at its real extent (checked with the symbol size patched to 0xA8); the do/while(0) blocks give the register choice and store order (as in func_00430454, its first-module copy). */
/* The real function is 0x00469374-0x0046941C (size 0xA8 = 168):
 * splat cut it at the data label D_00469400. */
#include "common.h"
#include "structs/game.h"

/* Start a level: check that the warp target exists, reset the multiplayer
 * options and switch the game state. */

extern u8 gScene[];

s32 func_004650B4(void *level, s32 a, s32 id, void *out, s32 flags);

void func_00469374(CEngineApp *game, s32 warpId) {
    u8 sp18[0x18];
    s32 id;

    id = warpId;

    if (func_004650B4(gScene, -1, id, sp18, 1) == 0) {
        id = 0;
    }
    D_8011ACB0.frames = 0;
    D_8011ACB0.seconds = 0;
    D_8011ACB0.minutes = 0;
    D_8011ACB0.hours = 0;
    D_8011ACB0.unk44 = 0;
    do {
        game->m_Settings.warpFlags = 0;
        game->m_Settings.matchMode = id;
    } while (0);
    do {
        game->m_Settings.fadeMode = 2;
        game->m_Settings.nextMode = 13;
    } while (0);
}
