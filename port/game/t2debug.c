/* t2debug.c: DEBUG-ONLY bring-up switches for `turok2 --game` (docs/port/STATUS.md, round 4 s2).
 *
 * The game cannot leave mode 0 (the Controller Pak check) until the PIF/Controller Pak is
 * modelled (s1, docs/port/INPUT.md). To follow the boot past it these switches move the engine
 * on by itself. They are bring-up aids, not options for players, and go away once input works:
 *
 *   --skip-pak-check  mode 0 ends as if "continue without saving" had been chosen: gPakCheckDone
 *                     is set once mode 0 has started, so EngineMode00_PakCheck_Retrace takes its
 *                     own exit (CEngineApp__SetupFadeTo(app, 1): the legal screen, then the
 *                     modes the game chooses itself).
 *   --start-mode N    once mode 0 has started, switch to mode N (CEngineApp__SetMode: through
 *                     mode 17 like any switch). 7 is the title screen.
 *
 * Called from game.cpp between retraces, while every game thread is blocked. */
#include "common.h"
#include "structs/game.h"
#include "globals/pak.h"
#include "globals/engine.h"

void CEngineApp__SetMode(CEngineApp *app, s32 mode);

int T2Port_EngineMode(void) {
    return gEngineApp.m_Settings.mode;
}

/* skipPak: 1 for --skip-pak-check; startMode: -1, or the mode for --start-mode. */
void T2Port_DebugSwitches(int skipPak, int startMode) {
    static int sStartDone;

    if (gEngineApp.m_Settings.mode != 0 || gPakCheckStarted == 0) {
        return;
    }
    if (startMode >= 0 && !sStartDone) {
        sStartDone = 1;
        CEngineApp__SetMode(&gEngineApp, startMode);
        return;
    }
    if (skipPak) {
        gPakCheckDone = 1;
    }
}
