/* The second VM module's copy of src/us/vm1/MotherBoss/func_0040AD60.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/scene.h"
#include "protos/scene.h"

/* Module init: collects up to 4 actors of type (0x44E, 9) into the module state, hooks their callbacks,
 * sets up the first phase (MotherBoss_SetupPhase0_2) and queues MotherBoss_OnIntroCinemaDone_2. */

extern void MotherBoss_Draw_2(void);
extern void MotherBoss_PartEffects_2(void);
extern void MotherBoss_SetupPhase0_2(void *obj, void *ai);
extern void MotherBoss_OnIntroCinemaDone_2(void);
extern s32 Cinema_SetNextCallbacks(s32 id, void *arg1, void *arg2);

void MotherBoss_StartIntro_2(void *obj, void *ai) {
    MotherBossState *state = &gMotherBoss;
    CGameObjectInstance *actor;
    s32 i;

    state->count = CScene__FindInstances(&gScene, 0x44E, 9, -1, state->actors, 4);
    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->m_AI.m_pfDraw = MotherBoss_Draw_2;
        actor->m_AI.m_pfEffects = MotherBoss_PartEffects_2;
    }
    MotherBoss_SetupPhase0_2(obj, ai);
    Cinema_SetNextCallbacks(0xC8, MotherBoss_OnIntroCinemaDone_2, NULL);
}
