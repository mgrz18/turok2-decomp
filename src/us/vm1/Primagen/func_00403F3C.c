#include "common.h"
#include "structs/boss.h"
#include "structs/game_object.h"
#include "globals/boss.h"
#include "globals/onscreen.h"
#include "globals/engine.h"
#include "protos/ai.h"





#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define AI_TurnHeadToTarget AI_TurnHeadToTarget_i
#endif
void AI_TurnHeadToTarget(CGameObjectInstance *actor, CAI *level, s32 a2);
void Animator_Update(void *light);
s16 WeightedTable_PickStatic(void *table);
void Primagen_SpawnTossedObject(s16 type, s32 a1, CVector3 offset);
void Primagen_UpdateHealth(CGameObjectInstance *actor, CAI *level);
void Primagen_UpdateSpawner(CGameObjectInstance *actor, CAI *level);
void COnScreen__SetBossPercent(void *a0, s32 percent);

#define COUNT_DOWN(t)              \
    if ((t) > 0.0f) {              \
        (t) -= gFrameIncrement;         \
    }

void Primagen_Step(CGameObjectInstance *actor, CAI *level) {
    CVector3 offset;
    s32 type;
    s32 i;

    AI_TurnHeadToTarget(actor, level, 0);
    if (level->m_pModeCurrent != NULL && level->m_pModeCurrent->m_pfCallback != NULL) {
        level->m_pModeCurrent->m_pfCallback(actor, level);
    }
    for (i = 0; i < 8; i++) {
        Animator_Update(&gLevelScript.anims[i]);
    }
    if (gLevelScript.bossState == 1) {
        level->m_vDesiredPos.x = gLevelScript.home.x;
        level->m_vDesiredPos.y = gLevelScript.home.y;
        level->m_vDesiredPos.z = gLevelScript.home.z;
    }
    COUNT_DOWN(gLevelScript.waveTimer);
    COUNT_DOWN(gLevelScript.script.timer);
    COUNT_DOWN(gLevelScript.script2.timer);
    COUNT_DOWN(gLevelScript.script3.timer);
    COUNT_DOWN(gLevelScript.spawnTimer);
    COUNT_DOWN(gLevelScript.swayTimer);
    COUNT_DOWN(gLevelScript.nextTimer);
    COUNT_DOWN(gLevelScript.timer3EC);
    if (gLevelScript.stage == 3) {
        if (gLevelScript.values[0][1] <= 0x95FF && gLevelScript.subStage == 0) {
            gLevelScript.subStage = 1;
        }
        if (gLevelScript.values[0][1] < 0x6400 && gLevelScript.subStage == 1) {
            gLevelScript.subStage = 2;
        }
        if (gLevelScript.values[0][1] < 0x3200 && gLevelScript.subStage == 2) {
            gLevelScript.subStage = 3;
        }
    }
    if (gLevelScript.timer3EC <= 0.0f) {
        offset.x = -2385.92f;
        offset.y = 0.0f;
        offset.z = 0.0f;
        if (gLevelScript.stage == 3) {
            type = WeightedTable_PickStatic(sPrimagenTossTableStage3);
        } else {
            type = WeightedTable_PickStatic(sPrimagenTossTable);
        }
        Primagen_SpawnTossedObject(type, 0x11D, offset);
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = -2324.48f;
        if (gLevelScript.stage == 3) {
            type = WeightedTable_PickStatic(sPrimagenTossTableStage3);
        } else {
            type = WeightedTable_PickStatic(sPrimagenTossTable);
        }
        Primagen_SpawnTossedObject(type, 0x11D, offset);
        offset.x = 2314.24f;
        offset.y = 0.0f;
        offset.z = 0.0f;
        if (gLevelScript.stage == 3) {
            type = WeightedTable_PickStatic(sPrimagenTossTable2Stage3);
        } else {
            type = WeightedTable_PickStatic(sPrimagenTossTable2);
        }
        Primagen_SpawnTossedObject(type, 0x11D, offset);
        offset.x = -460.8f;
        offset.y = 0.0f;
        offset.z = -460.8f;
        Primagen_SpawnTossedObject(WeightedTable_PickStatic(sPrimagenTossTable), 0x11D, offset);
        offset.x = 460.8f;
        offset.y = 0.0f;
        offset.z = -460.8f;
        Primagen_SpawnTossedObject(WeightedTable_PickStatic(sPrimagenTossTable), 0x11D, offset);
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 614.39996f;
        Primagen_SpawnTossedObject(WeightedTable_PickStatic(sPrimagenTossTable), 0x11D, offset);
        gLevelScript.timer3EC = 300.0f;
    }
    Primagen_UpdateHealth(actor, level);
    Primagen_UpdateSpawner(actor, level);
    COnScreen__SetBossPercent(&gEngineApp_m_OnScreen, gLevelScript.total * 100 / gLevelScript.bonus);
    if (actor->header.m_vPos.y < -204.79999f) {
        gLevelScript.bossMode = 11;
        gLevelScript.dest.x = 0.0f;
        gLevelScript.dest.y = 0.0f;
        gLevelScript.dest.z = 0.0f;
        CAI__SetMode(actor, level, 0x19);
    }
}
