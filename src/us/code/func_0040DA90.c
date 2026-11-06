#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

#define MAX(a, b) ((a) < (b) ? (b) : (a))

/* Update callback of the second boss: runs the anim callback, counts the hit timer down, runs the phase's
 * update, then updates the slots, the three movers and the boss' health bar. */


typedef struct Obj2 {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Obj2;

extern f32 gFrameIncrement; /* frame time */
extern BlindOnePhase sBlindOnePhases[];
extern u8 gEngineApp_m_OnScreen[];
extern void Animator_Update(void *slot);
extern void BlindOne_UpdateMover(BlindOneMover *mover);
extern s32 BlindOne_GetHealthPercent(BlindOneState *state);
extern void COnScreen__SetBossPercent(void *hud, s32 value);

void BlindOne_BossUpdate(void *obj, CAI *ai) {
    BlindOneState *state = &gBlindOne;
    s32 i;

    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ai->m_pModeCurrent->m_pfCallback(obj, ai);
    }
    ai->m_BlindOne.m_HitTimer = MAX(ai->m_BlindOne.m_HitTimer - gFrameIncrement, 0.0f);
    if (sBlindOnePhases[state->phase].update != NULL) {
        sBlindOnePhases[state->phase].update(state);
    }
    for (i = 0; i < state->faderCount; i++) {
        Animator_Update(&state->faders[i]);
    }
    BlindOne_UpdateMover(&state->movers[0]);
    BlindOne_UpdateMover(&state->movers[1]);
    BlindOne_UpdateMover(&state->movers[2]);
    COnScreen__SetBossPercent(gEngineApp_m_OnScreen, BlindOne_GetHealthPercent(state));
}
