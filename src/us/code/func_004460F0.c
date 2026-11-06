/* Second-module copy of BlindOne_PhaseMinionsUpdate (nonmatching/vm/BlindOne_PhaseMinionsUpdate.c), with its data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Second boss phase update: spawns a minion at a random point 1228.8-1536 around the boss while the phase has
 * some left (and fewer than its maximum are alive); once none are left and fewer than 5 are alive, steps the
 * script to the next phase. */


/* Plays a sequence of bit masks: every interval seconds, each set bit sends the matching actor (bits 0-3:
 * group 1, 4-7: group 2, 8-11: group 3, rotated by offset) to an attack state. */

extern BlindOnePhase sBlindOnePhases_2[];
extern u8 gScene_m_InstancePool[];
extern f32 Random_Blend(f32 min, f32 max);
extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */
extern void GameObject_TraceMove(CGameObjectInstance *obj, CVector3 pos, CVector3 *outPos, s32 *outRegion, s32 x, s32 y);
extern void *CInstancePool__Spawn(void *world, s32 model, s32 type, void *intelligence, s32 region, CVector3 pos, f32 rotY,
                           CVector3 vel, CVector3 scale, s32 *owner);

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

/* Spawns one minion around the boss when *toSpawn allows it and fewer than maxAlive are alive. */
static __inline__ void spawnMinion(BlindOneState *state, s32 *toSpawn, s32 *alive, f32 dist, s32 maxAlive,
                                   void *intelligence, CGameObjectInstance *obj, f32 angle) {
    CVector3 pos;
    CVector3 vel;
    CVector3 scale;
    s32 region;

    if (toSpawn != NULL && *toSpawn == 0) {
        return;
    }
    if (alive != NULL && maxAlive != -1 && *alive >= maxAlive) {
        return;
    }
    vel.z = vel.y = vel.x = 0.0f;
    scale.z = scale.y = scale.x = 0.5f;
    pos.x = obj->header.m_vPos.x + dist * func_002982F0(angle);
    pos.y = 0.0f;
    pos.z = obj->header.m_vPos.z + dist * func_002974C0(angle);
    GameObject_TraceMove(state->boss, pos, &pos, &region, 0, 0);
    if (CInstancePool__Spawn(gScene_m_InstancePool, -1, 0x1FF, intelligence, region, pos, angle, vel, scale, alive) && toSpawn != NULL) {
        (*toSpawn)--;
    }
}

void BlindOne_PhaseMinionsUpdate_2(BlindOneState *state) {
    spawnMinion(state, &state->toSpawn, &state->minionsAlive, Random_Blend(1228.7999f, 1536.0f), state->maxMinions,
                state->minionDefA, state->boss, Random_Blend(-3.141593f, 3.141593f));
    if (state->toSpawn == 0 && state->minionsAlive < 5) {
        nextPhase(state);
    }
}
