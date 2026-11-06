/* Second-module copy of MpMenu_StartTeamBlood (build/manual/W/MpMenu_StartTeamBlood.c), with its first-module addresses 0xE0 lower. */
#include "common.h"
#include "structs/game.h"
#include "structs/controller.h"
#include "globals/game.h"


extern s32 Controller_ConfirmPressed(ControllerProfile *ctl);
extern s32 Random_Next(void);
extern void func_00469374(void *a, s32 level);

/* Mp_SetupMatchRules, inlined. */
static __inline__ void StartMatch(f32 minutes, s32 a1, s32 started, s32 a3) {
    GameMatch *m = &gEngineApp_m_Settings.match;

    m->tag = started;
    m->teams = a3;
    m->scoreLimit = a1;
    m->over = 0;
    m->teamScore[0] = 0;
    m->teamScore[1] = 0;
    m->teamOver = 0;
    m->done = 0;
    m->timeLeft = minutes * 900.0f;
    if (m->timeLeft == 0.0f) {
        m->timeLeft = -1.0f;
    }
}

s32 MpMenu_StartTeamBlood_2(void) {
    GameSettings *setup = &gEngineApp_m_Settings;
    s32 level;
    s32 arg;
    s32 i;
    ControllerProfile *ctl;

    level = setup->teamArena;
    StartMatch(setup->teamTimeLimit, setup->teamScoreLimit, 0, 1);
    for (i = 3; i >= 0; i--) {
        setup->slots[i].m_nCharacter = 0;
    }
    for (i = 0, ctl = gControllers; i < 4; i++, ctl++) {
        if (Controller_ConfirmPressed(ctl)) {
            CPlayerOptions *slot = &gEngineApp_m_Settings.slots[i];

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
