#include "common.h"
#include "structs/vector.h"
#include "structs/game.h"
#include "structs/world.h"

/* Turok 2: the mode block sits at 0x23FD0 (see CEngineApp__UpdateCinemaMode). */

extern char gScene[];

s32 CScene__FindWarpDests(void *world, s32 arg1, s32 id, CWarpPoint *out, s32 max);

/* Resets the match state and asks the session for mode 13 at warp `warp` (0 if the
 * world has no such warp point). */
/* The two do/while(0) blocks (macro-like groups in the original?) give the ROM's register choice
 * and store order. */
void func_00430454(CEngineApp *session, s32 warp) {
    CWarpPoint point;

    if (CScene__FindWarpDests(gScene, -1, warp, &point, 1) == 0) {
        warp = 0;
    }
    D_8011ACB0.frames = 0;
    D_8011ACB0.seconds = 0;
    D_8011ACB0.minutes = 0;
    D_8011ACB0.hours = 0;
    D_8011ACB0.unk44 = 0;
    do {
        session->m_Settings.warpFlags = 0;
        session->m_Settings.matchMode = warp;
    } while (0);
    do {
        session->m_Settings.fadeMode = 2;
        session->m_Settings.nextMode = 13;
    } while (0);
}
