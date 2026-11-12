#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/menu.h"
#include "globals/render.h"
#include "globals/engine.h"
#include "globals/text.h"
#include "protos/player.h"

extern void CEngineApp__StartGameAtWarp(sptr, s32);
extern void CEngineApp__SetupFadeTo(sptr, s32);
extern void Sound__PlaySpeech(s32);

/* Scrolls the credits up; once past the last line, waits and leaves. */
void Credits_Update(sptr arg0) { /* the engine pointer-wide (sptr: s32 on the N64; the host cut it: crash after the credits) */
    f32 spacing;

    if (gCreditsDone != 0) {
        gCreditsTimeLeft -= gFrameIncrement;
        if (gCreditsTimeLeft <= 0.0f) {
            CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
            if (gEndingPrimagenDefeated != 0) {
                if (gFinalEnding == 0) {
                    CEngineApp__StartGameAtWarp(arg0, 0x1B57);
                    return;
                }
            } else if (gFinalEnding == 0) {
                return;
            }
            CEngineApp__SetupFadeTo(arg0, 1);
            gEngineApp_m_Settings.match.unk48 = 1;
        }
        return;
    }
    if (gScreenHeight >= 223) {
        gCreditsScroll -= 1.0f;
    } else {
        gCreditsScroll -= 0.5f;
    }
    spacing = gScreenHeight * 0.045045048f;
    if (gCreditsScroll <= -spacing) {
        sCreditsLine++;
        gCreditsScroll += spacing;
        if (gCreditsLines[sCreditsLine] == NULL) {
            gCreditsDone = 1;
            Sound__PlaySpeech(0x266);
            gCreditsTimeLeft = 67.5f;
        }
    }
}
