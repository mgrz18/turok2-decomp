#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Module init: collects up to 4 actors of type (0x44E, 9) into the module state, hooks their callbacks,
 * sets up the first phase (MotherBoss_SetupPhase0) and queues MotherBoss_OnIntroCinemaDone. */

extern u8 gScene[];
extern s32 CScene__FindInstances(void *container, s32 key0, s32 key1, s32 key2, CGameObjectInstance **out, s32 max);
extern void MotherBoss_Draw(void);
extern void MotherBoss_PartEffects(void);
extern void MotherBoss_SetupPhase0(void *obj, void *ai);
extern void MotherBoss_OnIntroCinemaDone(void);
extern s32 Cinema_SetNextCallbacks(s32 id, void *arg1, void *arg2);

void MotherBoss_StartIntro(void *obj, void *ai) {
    MotherBossState *state = &D_8012FB70;
    CGameObjectInstance *actor;
    s32 i;

    state->count = CScene__FindInstances(gScene, 0x44E, 9, -1, state->actors, 4);
    for (i = 0; i < state->count; i++) {
        actor = state->actors[i];
        actor->m_AI.m_pfDraw = MotherBoss_Draw;
        actor->m_AI.m_pfEffects = MotherBoss_PartEffects;
    }
    MotherBoss_SetupPhase0(obj, ai);
    Cinema_SetNextCallbacks(0xC8, MotherBoss_OnIntroCinemaDone, NULL);
}
