/* The second VM module's copy of src/us/vm1/MpMenu/func_00417BB0.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

void Mp_SetupMatchRules_2(f32 minutes, s32 a1, s32 started, s32 a3) {
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
