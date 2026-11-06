/* Second-module copy of BlindOne_Setup (in src/us/code/BlindOne_UpdateMover.c), with its data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

/* Second boss setup: collects its three groups of actors (types 0x454-0x456), sets their health and slots,
 * finds its two level objects, shows its health bar ("Blind") and enters the first phase of its script. */


/* Plays a sequence of bit masks: every interval seconds, each set bit sends the matching actor (bits 0-3:
 * group 1, 4-7: group 2, 8-11: group 3, rotated by offset) to an attack state. */

extern u8 gScene[];
extern u8 gEngineApp_m_OnScreen[];
extern s32 CScene__FindInstances(void *container, s32 key0, s32 key1, s32 key2, CGameObjectInstance **out, s32 max);
extern void ScriptAnimator_Start(void *slot, s32 arg1);
extern void *CScene__FindIntelligence(void *level, s32 arg1, s32 id);
extern void COnScreen__SetBoss(void *hud, CGameObjectInstance *obj, char *name);
extern void CScene__UnmarkGeneratorsSaved(void *level);

/* Steps the script: clears the movers and the actors' m_BlindOne.m_nPhaseHits, enters the next phase and runs its init. */
static __inline__ void nextPhase(BlindOneState *state) {
    s32 phase;
    s32 i;

    phase = (++state->script)->id;
    state->movers[0].seq = NULL;
    state->movers[1].seq = NULL;
    state->movers[2].seq = NULL;
    for (i = 0; i < state->count1; i++) {
        state->actors1[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < state->count2; i++) {
        state->actors2[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    for (i = 0; i < state->count3; i++) {
        state->actors3[i]->m_AI.m_BlindOne.m_nPhaseHits = 0;
    }
    state->phase = phase;
    if (sBlindOnePhases_2[phase].init != NULL) {
        sBlindOnePhases_2[phase].init(state);
    }
}

void BlindOne_Setup_2(BlindOneState *state) {
    CGameObjectInstance *obj;
    CGameObjectInstance *actor;
    s32 i;

    state->count1 = CScene__FindInstances(gScene, 0x454, 1, -1, state->actors1, 4);
    state->count2 = CScene__FindInstances(gScene, 0x455, 1, -1, state->actors2, 4);
    state->count3 = CScene__FindInstances(gScene, 0x456, 1, -1, state->actors3, 3);
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
        ScriptAnimator_Start(&state->faders[i], 0);
    }
    state->minionDefA = CScene__FindIntelligence(gScene, 1, 0x1CE9);
    state->minionDefB = CScene__FindIntelligence(gScene, 1, 0x1CEA);
    state->toSpawn = 0;
    state->minionsAlive = 0;
    state->maxMinions = 0;
    state->alive1 = 0;
    state->alive2 = 0;
    state->alive3 = 0;
    COnScreen__SetBoss(gEngineApp_m_OnScreen, state->boss, "Blind");
    CScene__UnmarkGeneratorsSaved(gScene);
    nextPhase(state);
}


