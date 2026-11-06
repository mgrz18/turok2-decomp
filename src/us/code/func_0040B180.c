#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Clears the two scales (m_Mother) of every actor of the module, resets the module (MotherBoss_SetupPhase1) and queues MotherBoss_OnPhase1CinemaDone. */

extern void MotherBoss_SetupPhase1(void *obj, void *ai);
extern void MotherBoss_OnPhase1CinemaDone(void);
extern s32 Cinema_SetNextCallbacks(s32 id, void *arg1, void *arg2);

void MotherBoss_StartPhase1Cinema(void *obj, void *ai) {
    MotherBossState *state = &D_8012FB70;
    CGameObjectInstance *actor;
    s32 i;

    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->m_AI.m_Mother.m_GrowScale = 0;
        actor->m_AI.m_Mother.m_PulseScale = 0;
    }
    MotherBoss_SetupPhase1(obj, ai);
    Cinema_SetNextCallbacks(0xD2, MotherBoss_OnPhase1CinemaDone, NULL);
}
