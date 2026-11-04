#include "common.h"
#include "structs/vector.h"
#include "structs/game.h"
#include "structs/world.h"

/* Turok 2: the mode block sits at 0x23FD0 (see func_00288E58). */
typedef struct GameSession {
    char pad0[0x23FDC];
    s32 target;                /* 0x23FDC */
    char pad23FE0;
    s8 phase;                  /* 0x23FE1 */
    char pad23FE2[0x23FF8 - 0x23FE2];
    s32 warp;                  /* 0x23FF8 */
    s32 unk23FFC;              /* 0x23FFC */
} GameSession;

extern char D_800F7078[];

s32 func_0042C194(void *world, s32 arg1, s32 id, CWarpPoint *out, s32 max);

/* Resets the match state and asks the session for mode 13 at warp `warp` (0 if the
 * world has no such warp point). */
/* The two do/while(0) blocks (macro-like groups in the original?) give the ROM's register choice
 * and store order. */
void func_00430454(GameSession *session, s32 warp) {
    CWarpPoint point;

    if (func_0042C194(D_800F7078, -1, warp, &point, 1) == 0) {
        warp = 0;
    }
    D_8011ACB0.frames = 0;
    D_8011ACB0.seconds = 0;
    D_8011ACB0.minutes = 0;
    D_8011ACB0.hours = 0;
    D_8011ACB0.unk44 = 0;
    do {
        session->unk23FFC = 0;
        session->warp = warp;
    } while (0);
    do {
        session->phase = 2;
        session->target = 13;
    } while (0);
}
