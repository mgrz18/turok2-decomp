/* Second-module copy of BlindOne_BossUpdate (src/us/code/BlindOne_BossUpdate.c), with its string/data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

extern BlindOneState D_80130200;

#define MAX(a, b) ((a) < (b) ? (b) : (a))

/* Update callback of the second boss: runs the anim callback, counts the hit timer down, runs the phase's
 * update, then updates the slots, the three movers and the boss' health bar. */


extern f32 D_800B6D28; /* frame time */
extern BlindOnePhase D_0043232C[];
extern u8 D_801100F0[];
extern void Animator_Update(void *slot);
extern void BlindOne_UpdateMover(BlindOneMover *mover);
extern s32 BlindOne_GetHealthPercent(BlindOneState *state);
extern void COnScreen__SetBossPercent(void *hud, s32 value);

void BlindOne_BossUpdate_2(void *obj, CAI *ai) {
    BlindOneState *state = &D_80130200;
    s32 i;

    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ai->m_pModeCurrent->m_pfCallback(obj, ai);
    }
    ai->m_BlindOne.m_HitTimer = MAX(ai->m_BlindOne.m_HitTimer - D_800B6D28, 0.0f);
    if (D_0043232C[state->phase].update != NULL) {
        D_0043232C[state->phase].update(state);
    }
    for (i = 0; i < state->faderCount; i++) {
        Animator_Update(&state->faders[i]);
    }
    BlindOne_UpdateMover(&state->movers[0]);
    BlindOne_UpdateMover(&state->movers[1]);
    BlindOne_UpdateMover(&state->movers[2]);
    COnScreen__SetBossPercent(D_801100F0, BlindOne_GetHealthPercent(state));
}
