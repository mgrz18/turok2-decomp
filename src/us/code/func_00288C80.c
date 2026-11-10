#include "common.h"
#include "structs/game.h"
#include "globals/frame.h"
#include "globals/engine.h"

s32 CEngineApp__Update(sptr);
s32 SavedPlayers_UpdateTimer(void);

/* The frame/game state block passed to the per-mode handlers (the
 * handler table gEngineModeTable has 0xC-byte rows indexed by mode). */

void CEngineApp__Retrace(CEngineApp *arg0) {
    void (*temp_v0)(struct CEngineApp *);

    if (gFirstFrame != 0) {
        gFirstFrame = 0;
    }
    SavedPlayers_UpdateTimer();
    temp_v0 = gEngineModeTable[arg0->m_Settings.mode].retrace;
    if (temp_v0 != NULL) {
        temp_v0(arg0);
    }
    if ((arg0->m_Settings.mode != 0x11) && (gDisplayListCount != 0)) {
        gDisplayListCount -= 1;
        CEngineApp__Update((sptr) arg0);
    }
    gRetraceCount += 1;
}
