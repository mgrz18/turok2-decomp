#include "common.h"
#include "structs/game.h"
#include "globals/engine.h"
#include "globals/text.h"

extern s32 gCreditsDone;              /* credits finished */
extern f32 gCreditsTimeLeft;              /* time left before leaving the credits */
extern s32 gEngineApp_m_Settings_match_unk48;
extern s32 gScreenHeight;              /* screen height */
extern s32 sCreditsLine;              /* first credits line on screen */
extern f32 gCreditsScroll;              /* scroll position of that line */

extern void CPlayerPool__GetFirstActive(void *);
extern void CEngineApp__StartGameAtWarp(s32, s32);
extern void CEngineApp__SetupFadeTo(s32, s32);
extern void Sound__PlaySpeech(s32);

/* Scrolls the credits up; once past the last line, waits and leaves. */
void Credits_Update(s32 arg0) {
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
            gEngineApp_m_Settings_match_unk48 = 1;
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
