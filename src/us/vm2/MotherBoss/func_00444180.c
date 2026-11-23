/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040B180.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

/* Clears the two scales (m_Mother) of every actor of the module, resets the module (MotherBoss_SetupPhase1_2) and queues MotherBoss_OnPhase1CinemaDone_2. */

extern void MotherBoss_SetupPhase1_2(void *obj, void *ai);
extern void MotherBoss_OnPhase1CinemaDone_2(void);
extern s32 Cinema_SetNextCallbacks(s32 id, void *arg1, void *arg2);

void MotherBoss_StartPhase1Cinema_2(void *obj, void *ai) {
    MotherBossState *state = &gMotherBoss;
    CGameObjectInstance *actor;
    s32 i;

    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->m_AI.m_Mother.m_GrowScale = 0;
        actor->m_AI.m_Mother.m_PulseScale = 0;
    }
    MotherBoss_SetupPhase1_2(obj, ai);
    Cinema_SetNextCallbacks(0xD2, MotherBoss_OnPhase1CinemaDone_2, NULL);
}
