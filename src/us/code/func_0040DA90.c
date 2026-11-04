#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

#define MAX(a, b) ((a) < (b) ? (b) : (a))

/* Update callback of the second boss: runs the anim callback, counts the hit timer down, runs the phase's
 * update, then updates the slots, the three movers and the boss' health bar. */


typedef struct Obj2 {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Obj2;

extern BlindOneState D_80130200;
extern f32 D_800B6D28; /* frame time */
extern BlindOnePhase D_0043240C[];
extern u8 D_801100F0[];
extern void func_00235510(void *slot);
extern void func_0040C850(BlindOneMover *mover);
extern s32 func_0040CB28(BlindOneState *state);
extern void func_0027AD88(void *hud, s32 value);

void func_0040DA90(void *obj, CAI *ai) {
    BlindOneState *state = &D_80130200;
    s32 i;

    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ai->m_pModeCurrent->m_pfCallback(obj, ai);
    }
    ai->m_VarF32[3] = MAX(ai->m_VarF32[3] - D_800B6D28, 0.0f);
    if (D_0043240C[state->phase].update != NULL) {
        D_0043240C[state->phase].update(state);
    }
    for (i = 0; i < state->faderCount; i++) {
        func_00235510(&state->faders[i]);
    }
    func_0040C850(&state->movers[0]);
    func_0040C850(&state->movers[1]);
    func_0040C850(&state->movers[2]);
    func_0027AD88(D_801100F0, func_0040CB28(state));
}
