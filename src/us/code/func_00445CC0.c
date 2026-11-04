/* Second-module copy of func_0040CCC0 (in src/us/code/func_0040C850.c), with its data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Second boss setup: collects its three groups of actors (types 0x454-0x456), sets their health and slots,
 * finds its two level objects, shows its health bar ("Blind") and enters the first phase of its script. */


/* Plays a sequence of bit masks: every interval seconds, each set bit sends the matching actor (bits 0-3:
 * group 1, 4-7: group 2, 8-11: group 3, rotated by offset) to an attack state. */

extern BlindOnePhase D_0043232C[];
extern u8 D_800F7078[];
extern u8 D_801100F0[];
extern s32 func_00225F6C(void *container, s32 key0, s32 key1, s32 key2, CGameObjectInstance **out, s32 max);
extern void func_002362E4(void *slot, s32 arg1);
extern void *func_00225EBC(void *level, s32 arg1, s32 id);
extern void func_0027AD24(void *hud, CGameObjectInstance *obj, char *name);
extern void func_00225D0C(void *level);

/* Steps the script: clears the movers and the actors' m_BlindOne.m_PhaseState, enters the next phase and runs its init. */
static __inline__ void nextPhase(BlindOneState *state) {
    s32 phase;
    s32 i;

    phase = (++state->script)->id;
    state->movers[0].seq = NULL;
    state->movers[1].seq = NULL;
    state->movers[2].seq = NULL;
    for (i = 0; i < state->count1; i++) {
        state->actors1[i]->m_AI.m_BlindOne.m_PhaseState = 0;
    }
    for (i = 0; i < state->count2; i++) {
        state->actors2[i]->m_AI.m_BlindOne.m_PhaseState = 0;
    }
    for (i = 0; i < state->count3; i++) {
        state->actors3[i]->m_AI.m_BlindOne.m_PhaseState = 0;
    }
    state->phase = phase;
    if (D_0043232C[phase].init != NULL) {
        D_0043232C[phase].init(state);
    }
}

void func_00445CC0(BlindOneState *state) {
    CGameObjectInstance *obj;
    CGameObjectInstance *actor;
    s32 i;

    state->count1 = func_00225F6C(D_800F7078, 0x454, 1, -1, state->actors1, 4);
    state->count2 = func_00225F6C(D_800F7078, 0x455, 1, -1, state->actors2, 4);
    state->count3 = func_00225F6C(D_800F7078, 0x456, 1, -1, state->actors3, 3);
    obj = state->boss;
    state->faderCount = 0;
    obj->m_AI.m_Health = obj->m_AI.m_InitialHealth = 0x3200;
    obj->m_AI.m_BlindOne.m_nFader = state->faderCount++;
    for (i = 0; i < state->count1; i++) {
        actor = state->actors1[i];
        actor->m_AI.m_Health = 0;
        actor->m_AI.m_InitialHealth = 0x6400;
        actor->m_AI.m_BlindOne.m_nGroupIndex = i;
        actor->m_AI.m_BlindOne.m_nFader = state->faderCount++;
    }
    for (i = 0; i < state->count2; i++) {
        actor = state->actors2[i];
        actor->m_AI.m_Health = 0;
        actor->m_AI.m_InitialHealth = 0x5000;
        actor->m_AI.m_BlindOne.m_nGroupIndex = i;
        actor->m_AI.m_BlindOne.m_nFader = state->faderCount++;
    }
    for (i = 0; i < state->count3; i++) {
        actor = state->actors3[i];
        actor->m_AI.m_Health = 0;
        actor->m_AI.m_InitialHealth = 0x6400;
        actor->m_AI.m_BlindOne.m_nGroupIndex = i;
        actor->m_AI.m_BlindOne.m_nFader = state->faderCount++;
    }
    for (i = 0; i < state->faderCount; i++) {
        func_002362E4(&state->faders[i], 0);
    }
    state->minionDefA = func_00225EBC(D_800F7078, 1, 0x1CE9);
    state->minionDefB = func_00225EBC(D_800F7078, 1, 0x1CEA);
    state->toSpawn = 0;
    state->minionsAlive = 0;
    state->maxMinions = 0;
    state->alive1 = 0;
    state->alive2 = 0;
    state->alive3 = 0;
    func_0027AD24(D_801100F0, state->boss, "Blind");
    func_00225D0C(D_800F7078);
    nextPhase(state);
}


