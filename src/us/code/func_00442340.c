#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/scene.h"

/* Module init: find the module actors (0x1DB1) and the controller (0x1DB2), set up the AI (mode table and callbacks),
 * then attach every slot to the object. */

extern s32 D_800F1F10[];
extern u8 gScene[];
extern u8 sMotherBossModeTable_2[];
extern s32 CScene__FindInstances(void *world, s32 key0, s32 key1, s32 key2, void *out, s32 max);
extern void *CScene__FindIntelligence(void *world, s32 a1, s32 id);
extern void ScriptAnimator_Start(void *slot, void *obj);
extern void CAI__SetMode(void *obj, CAI *ai, s32 event);
extern void EnemyAI_HeadNodeCallback();
extern void func_00441FF8();
extern void MotherBoss_Step_2();
extern void MotherBoss_Draw_2();
extern void MotherBoss_Collision_2();

void MotherBoss_Init_2(CGameObjectInstance *obj, CAI *ai) {
    MotherBossState *state = &gMotherBoss;
    s32 i;

    gScene_m_LevelID = 0x1DB1;
    state->pickupTimer = 0;
    state->dropTimer = 0;
    state->partCount = CScene__FindInstances(gScene, 0x650, 10, 0x1DB1, state->parts, 6);
    for (i = 0; i < state->partCount; i++) {
        state->parts[i]->m_AI.m_Mother.m_bActive = 1;
    }
    state->partsLeft = state->partCount;
    CScene__FindInstances(gScene, 0x650, 10, 0x1DB2, &state->controller, 1);
    state->controller->m_AI.m_Mother.m_bActive = 1;
    state->intel = CScene__FindIntelligence(gScene, 1, 0x1DB1);
    ai->m_pModeTable = (struct CAIModeInfo *) sMotherBossModeTable_2;
    ai->m_pfStep = MotherBoss_Step_2;
    ai->m_pfHurt = (u8 *)func_00441FF8 + 8;
    ai->m_pfEffects = EnemyAI_HeadNodeCallback;
    ai->m_pfCollision = MotherBoss_Collision_2;
    ai->m_pfDraw = MotherBoss_Draw_2;
    CAI__SetMode(obj, ai, 0);
    gMotherBoss.obj = obj;
    D_800F1F10[0] &= ~0x300;
    obj->m_dwFlags |= 0x04000000;
    for (i = 0; i < 15; i++) {
        ScriptAnimator_Start(&gMotherBoss.slots[i].anim, obj);
    }
}
