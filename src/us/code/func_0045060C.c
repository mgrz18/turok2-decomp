/* Second-module copy of MpMenu_StartBloodLust (build/manual/W/MpMenu_StartBloodLust.c), with its first-module addresses 0xE0 lower. */
#include "common.h"
#include "structs/game.h"
#include "structs/controller.h"

extern CPlayerOptions gEngineApp_m_Settings_slots[4]; /* gEngineApp_m_Settings.slots */

extern s32 Controller_ConfirmPressed(ControllerProfile *ctl);
extern s32 Random_Next(void);
extern void func_00469374(void *a, s32 level);

/* Mp_SetupMatchRules, inlined; here written on the global (the copies in MpMenu_StartFragTag_2/MpMenu_StartTeamBlood_2 go
 * through a MatchState pointer, which this one does not match with). */
static __inline__ void StartMatch(f32 minutes, s32 a1, s32 started, s32 a3) {
    gEngineApp_m_Settings.match.tag = started;
    gEngineApp_m_Settings.match.teams = a3;
    gEngineApp_m_Settings.match.scoreLimit = a1;
    gEngineApp_m_Settings.match.over = 0;
    gEngineApp_m_Settings.match.teamScore[0] = 0;
    gEngineApp_m_Settings.match.teamScore[1] = 0;
    gEngineApp_m_Settings.match.teamOver = 0;
    gEngineApp_m_Settings.match.done = 0;
    gEngineApp_m_Settings.match.timeLeft = minutes * 900.0f;
    if (gEngineApp_m_Settings.match.timeLeft == 0.0f) {
        gEngineApp_m_Settings.match.timeLeft = -1.0f;
    }
}

s32 MpMenu_StartBloodLust_2(void) {
    GameSettings *setup = &gEngineApp_m_Settings;
    s32 level;
    s32 arg;
    s32 i;
    ControllerProfile *ctl;

    level = setup->dmArena;
    StartMatch(setup->dmTimeLimit, setup->dmScoreLimit, 0, 0);
    for (i = 3; i >= 0; i--) {
        setup->slots[i].m_nCharacter = 0;
    }
    for (i = 0, ctl = gControllers; i < 4; i++, ctl++) {
        if (Controller_ConfirmPressed(ctl)) {
            CPlayerOptions *slot = &gEngineApp_m_Settings_slots[i];

            slot->m_bActive = 1;
            slot->m_nController = i;
            break;
        }
    }
    if (level != 0) {
        if ((u32) (level - 1) < 13) {
            arg = level + 0x2328;
        } else {
            arg = Random_Next() % 13 + 0x2329;
        }
    } else {
        arg = Random_Next() % 13 + 0x2329;
    }
    func_00469374(&gEngineApp, arg);
    return 1;
}
