#include "common.h"
#include "structs/game.h"
#include "globals/frame.h"
#include "globals/engine.h"

s32 Music_SetTrack(s32);
s32 Cinema_Stop(void);
s32 Controller_SetRumbleOptionAll(s32);
s32 Demo_End(void);

s32 Sound__StopAllExcept(s32);                     /* extern */
s32 Music_FadeOut();                            /* extern */

/* The frame/game state block passed to the per-mode handlers (the
 * handler table gEngineModeTable has 0xC-byte rows indexed by mode). */

void EngineMode17_Switch_Retrace(CEngineApp *arg0) {
    void (*temp_v0)(struct CEngineApp *);
    s32 temp_v1;

    if ((gDisplayListCount == 2) && (arg0->m_Settings.unk1C8 == 0)) {
        arg0->m_Settings.modeTimer = 0;
        arg0->m_Settings.mode = arg0->m_Settings.nextMode;
        Cinema_Stop();
        Sound__StopAllExcept(-1);
        temp_v1 = arg0->m_Settings.nextMode;
        if ((temp_v1 != 8) && (temp_v1 != 0xC)) {
            if ((arg0->m_Settings.prevMode != 8) || (arg0->m_Settings.mode != 0xD)) {
                Music_SetTrack(-1);
                Music_FadeOut();
                goto block_7;
            }
        } else {
block_7:
            if (arg0->m_Settings.mode != 0xD) {
                Demo_End();
            }
        }
        Controller_SetRumbleOptionAll(0);
        temp_v0 = gEngineModeTable[arg0->m_Settings.mode].init;
        if (temp_v0 != NULL) {
            temp_v0(arg0);
        }
    }
}
