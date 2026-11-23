/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00294BB0.c, func_00294BB0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

/* Turok 2 layouts (Rage Wars offsets in parentheses). */
typedef struct B21GameBlk {
    u8 pad0[0x44];
    s32 unk44;          /* 0x44 (0x88) */
} B21GameBlk;
s32 MenuPool_CountOpen(void *a);
s32 Cinema_PathPlaying();
s32 Cinema_GetScriptId();
s32 Cinema_IsFinished();
void Cinema_StopAtEnd();
s32 CEngineApp__CheckEndCinema(CEngineApp *g) {
    B21GameBlk *b;
    s32 r;
    if (MenuPool_CountOpen(gEngineApp_m_Game.m_MenuPool) == 0) {
        if (Cinema_PathPlaying()) {
            if (Cinema_GetScriptId() == 0x78) {
                b = (B21GameBlk *) &gEngineApp_m_Game.m_Settings.match;
                if (b->unk44 != 0) {
                    goto check;
                }
                if (g->m_Settings.paused == 0) {
                    b->unk44 = 1;
                    Cinema_StopAtEnd();
                }
            }
        }
    }
    if (gEngineApp_m_Settings.match.unk44 != 0) {
check:
        r = Cinema_IsFinished();
        if (r) return 1;
    }
    return 0;
}
