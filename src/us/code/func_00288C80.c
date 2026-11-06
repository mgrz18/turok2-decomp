#include "common.h"
#include "structs/game.h"
#include "globals/frame.h"

s32 CEngineApp__Update(s32);
s32 SavedPlayers_UpdateTimer(void);

extern s32 gFirstFrame;
extern s32 gRetraceCount;
extern s32 D_800B6D7C;

/* The frame/game state block passed to the per-mode handlers (the
 * handler table gEngineModeTable/D_800B6D7C has 0xC-byte rows indexed by mode). */

void CEngineApp__Retrace(CEngineApp *arg0) {
    s32 (*temp_v0)(void *);

    if (gFirstFrame != 0) {
        gFirstFrame = 0;
    }
    SavedPlayers_UpdateTimer();
    temp_v0 = *(((__typeof__(&D_800B6D7C))((s8 *)&D_800B6D7C + (arg0->m_Settings.mode * 0xC))));
    if (temp_v0 != NULL) {
        temp_v0(arg0);
    }
    if ((arg0->m_Settings.mode != 0x11) && (gDisplayListCount != 0)) {
        gDisplayListCount -= 1;
        CEngineApp__Update((s32) arg0);
    }
    gRetraceCount += 1;
}
