#include "common.h"
#include "structs/game.h"
#include "structs/controller.h"

extern CPlayerOptions gEngineApp_m_Settings_slots[4]; /* gEngineApp_m_Settings.slots */

extern s32 Controller_ConfirmPressed(ControllerProfile *ctl);
extern s32 Random_Next(void);
extern void func_00430454(void *a, s32 level);

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

s32 MpMenu_StartFragTag(void) {
    GameSettings *setup = &gEngineApp_m_Settings;
    s32 level;
    s32 arg;
    s32 i;
    ControllerProfile *ctl;

    level = setup->tagArena;
    StartMatch(setup->tagTimeLimit, setup->tagScoreLimit, 1, 0);
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
        if ((u32) (level - 1) < 6) {
            arg = level + 0x2336;
        } else {
            arg = Random_Next() % 6 + 0x2337;
        }
    } else {
        arg = Random_Next() % 6 + 0x2337;
    }
    func_00430454(&gEngineApp, arg);
    return 1;
}
