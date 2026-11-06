/* Second-module copy of BlindOne_BossUpdate (src/us/code/BlindOne_BossUpdate.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOneState gBlindOne;

#define MAX(a, b) ((a) < (b) ? (b) : (a))

/* Update callback of the second boss: runs the anim callback, counts the hit timer down, runs the phase's
 * update, then updates the slots, the three movers and the boss' health bar. */


extern f32 gFrameIncrement; /* frame time */
extern BlindOnePhase sBlindOnePhases_2[];
extern u8 gEngineApp_m_OnScreen[];
extern void Animator_Update(void *slot);
extern void BlindOne_UpdateMover_2(BlindOneMover *mover);
extern s32 BlindOne_GetHealthPercent_2(BlindOneState *state);
extern void COnScreen__SetBossPercent(void *hud, s32 value);

void BlindOne_BossUpdate_2(void *obj, CAI *ai) {
    BlindOneState *state = &gBlindOne;
    s32 i;

    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ai->m_pModeCurrent->m_pfCallback(obj, ai);
    }
    ai->m_BlindOne.m_HitTimer = MAX(ai->m_BlindOne.m_HitTimer - gFrameIncrement, 0.0f);
    if (sBlindOnePhases_2[state->phase].update != NULL) {
        sBlindOnePhases_2[state->phase].update(state);
    }
    for (i = 0; i < state->faderCount; i++) {
        Animator_Update(&state->faders[i]);
    }
    BlindOne_UpdateMover_2(&state->movers[0]);
    BlindOne_UpdateMover_2(&state->movers[1]);
    BlindOne_UpdateMover_2(&state->movers[2]);
    COnScreen__SetBossPercent(gEngineApp_m_OnScreen, BlindOne_GetHealthPercent_2(state));
}
