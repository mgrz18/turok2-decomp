/* Second-module copy of func_0040CCC0 (in src/us/code/func_0040C850.c), with its data addresses 0xE0 lower. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Second boss setup: collects its three groups of actors (types 0x454-0x456), sets their health and slots,
 * finds its two level objects, shows its health bar ("Blind") and enters the first phase of its script. */

struct BlindOneState;

typedef struct {
    /* 0x00 */ void (*init)(struct BlindOneState *state);
    /* 0x04 */ void (*update)(struct BlindOneState *state);
} PhaseFuncs;

typedef struct Obj2 {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[4];
    /* 0x144 */ s32 health;
    /* 0x148 */ s32 maxHealth;
    /* 0x14C */ u8 pad14C[0x254 - 0x14C];
    /* 0x254 */ s32 slot;
} Obj2;

extern PhaseFuncs D_0043232C[];
extern u8 D_800F7078[];
extern u8 D_801100F0[];
extern s32 func_00225F6C(void *container, s32 key0, s32 key1, s32 key2, CGameObjectInstance **out, s32 max);
extern void func_002362E4(void *slot, s32 arg1);
extern void *func_00225EBC(void *level, s32 arg1, s32 id);
extern void func_0027AD24(void *hud, Obj2 *obj, char *name);
extern void func_00225D0C(void *level);

/* Steps the script: clears the movers and the actors' 0x24C, enters the next phase and runs its init. */
static __inline__ void nextPhase(BlindOneState *state) {
    s32 phase;
    s32 i;

    phase = (++state->script)->id;
    state->movers[0].seq = NULL;
    state->movers[1].seq = NULL;
    state->movers[2].seq = NULL;
    for (i = 0; i < state->count1; i++) {
        state->actors1[i]->m_AI.m_VarBuffer[0] = 0;
    }
    for (i = 0; i < state->count2; i++) {
        state->actors2[i]->m_AI.m_VarBuffer[0] = 0;
    }
    for (i = 0; i < state->count3; i++) {
        state->actors3[i]->m_AI.m_VarBuffer[0] = 0;
    }
    state->phase = phase;
    if (D_0043232C[phase].init != NULL) {
        D_0043232C[phase].init(state);
    }
}

void func_00445CC0(BlindOneState *state) {
    Obj2 *obj;
    CGameObjectInstance *actor;
    s32 i;

    state->count1 = func_00225F6C(D_800F7078, 0x454, 1, -1, state->actors1, 4);
    state->count2 = func_00225F6C(D_800F7078, 0x455, 1, -1, state->actors2, 4);
    state->count3 = func_00225F6C(D_800F7078, 0x456, 1, -1, state->actors3, 3);
    obj = state->obj;
    state->slotCount = 0;
    obj->health = obj->maxHealth = 0x3200;
    obj->slot = state->slotCount++;
    for (i = 0; i < state->count1; i++) {
        actor = state->actors1[i];
        actor->m_AI.m_Health = 0;
        actor->m_AI.m_InitialHealth = 0x6400;
        actor->m_AI.m_VarBuffer[1] = i;
        actor->m_AI.m_VarBuffer[2] = state->slotCount++;
    }
    for (i = 0; i < state->count2; i++) {
        actor = state->actors2[i];
        actor->m_AI.m_Health = 0;
        actor->m_AI.m_InitialHealth = 0x5000;
        actor->m_AI.m_VarBuffer[1] = i;
        actor->m_AI.m_VarBuffer[2] = state->slotCount++;
    }
    for (i = 0; i < state->count3; i++) {
        actor = state->actors3[i];
        actor->m_AI.m_Health = 0;
        actor->m_AI.m_InitialHealth = 0x6400;
        actor->m_AI.m_VarBuffer[1] = i;
        actor->m_AI.m_VarBuffer[2] = state->slotCount++;
    }
    for (i = 0; i < state->slotCount; i++) {
        func_002362E4(&state->slots[i], 0);
    }
    state->minionIntel = func_00225EBC(D_800F7078, 1, 0x1CE9);
    state->minionIntel2 = func_00225EBC(D_800F7078, 1, 0x1CEA);
    state->toSpawn = 0;
    state->minionsAlive = 0;
    state->maxMinions = 0;
    state->alive1 = 0;
    state->alive2 = 0;
    state->alive3 = 0;
    func_0027AD24(D_801100F0, state->obj, "Blind");
    func_00225D0C(D_800F7078);
    nextPhase(state);
}

