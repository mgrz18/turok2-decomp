/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00294BB0.c, func_00294BB0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game.h"

/* Turok 2 layouts (Rage Wars offsets in parentheses). */
typedef struct B21GameBlk {
    u8 pad0[0x44];
    s32 unk44;          /* 0x44 (0x88) */
} B21GameBlk;
typedef struct B21World {
    u8 pad0[0x5DC];
    u8 sub5DC[4];       /* 0x5DC */
    u8 pad5E0[0xE80];
    B21GameBlk blk;     /* 0x1460 (0x1860) */
} B21World;
extern B21World D_80119850;
extern s32 gEngineApp_m_Settings_match_unk44;
s32 MenuPool_CountOpen(void *a);
s32 Cinema_PathPlaying();
s32 Cinema_GetScriptId();
s32 Cinema_IsFinished();
void Cinema_StopAtEnd();
s32 func_00289F30(CEngineApp *g) {
    B21GameBlk *b;
    s32 r;
    if (MenuPool_CountOpen(D_80119850.sub5DC) == 0) {
        if (Cinema_PathPlaying()) {
            if (Cinema_GetScriptId() == 0x78) {
                b = &D_80119850.blk;
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
    if (gEngineApp_m_Settings_match_unk44 != 0) {
check:
        r = Cinema_IsFinished();
        if (r) return 1;
    }
    return 0;
}
