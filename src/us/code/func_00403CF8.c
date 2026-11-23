#include "common.h"
#include "structs/boss.h"
#include "structs/game_object.h"
#include "globals/boss.h"
#include "globals/scene.h"
#include "globals/boss_ai.h"
#include "protos/scene.h"
#include "protos/ai.h"
#include "structs/vector.h"




void ScriptAnimator_Start(void *light, void *actor);
void Primagen_SetStage(s32 mode);
void Primagen_SetSpawnScript(s32 a0);
void Primagen_SetAttackScript(s32 a0);
void Primagen_Step();
void Primagen_Hurt();
void Primagen_Effects();
void Primagen_Draw();

void Primagen_InitAI(CGameObjectInstance *actor, CAI *level) {
    s32 i;

    gScene_m_LevelID = 0x1DB2;
    level->m_pModeTable = sPrimagenModeTable;
    level->m_pfStep = Primagen_Step;
    level->m_pfHurt = Primagen_Hurt;
    level->m_pfEffects = Primagen_Effects;
    level->m_pfDraw = Primagen_Draw;
    level->m_VisBits = -1;
    gLevelScript.actor = actor;
    CScene__FindInstances(&gScene, -1, 6, 0xC8, &gLevelScript.found[1], 1);
    CScene__FindInstances(&gScene, -1, 6, 0xC9, &gLevelScript.found[0], 1);
    CScene__FindInstances(&gScene, -1, 6, 0xCA, &gLevelScript.found[2], 1);
    CScene__FindInstances(&gScene, -1, 6, 0xCB, &gLevelScript.found[3], 1);
    CScene__FindInstances(&gScene, -1, 6, 0xCC, &gLevelScript.found[4], 1);
    gLevelScript.intel = CScene__FindIntelligence(&gScene, 1, 0x1DB2);
    for (i = 0; i < 8; i++) {
        ScriptAnimator_Start(&gLevelScript.anims[i], actor);
    }
    Primagen_SetStage(1);
    Primagen_SetSpawnScript(1);
    gLevelScript.bossMode = 1;
    gLevelScript.bossState = 1;
    gLevelScript.spawnsLeft = 0;
    gLevelScript.enemiesAlive = 0;
    gLevelScript.resetRegion = 1;
    gLevelScript.doorsOpen = 0;
    gLevelScript.point.y = 0;
    gLevelScript.speed = 100.0f;
    gLevelScript.point.x = 5.0f;
    gLevelScript.point.z = 180.0f;
    Primagen_SetAttackScript(1);
    gLevelScript.script.timer = 0;
    gLevelScript.script2.timer = 0;
    gLevelScript.spawnStep = 0;
    gLevelScript.subStage = 0;
    gLevelScript.waveTimer = 0;
    gLevelScript.spawnTimer = 0;
    gLevelScript.script3.timer = 0;
    gLevelScript.timer3EC = 0;
    gLevelScript.home.x = actor->header.m_vPos.x;
    gLevelScript.home.y = actor->header.m_vPos.y;
    gLevelScript.home.z = actor->header.m_vPos.z;
    gLevelScript_path = 0;
    sPrimagenPathPoint = 0;
    CAI__SetMode(actor, level, 0);
    level->m_Action = 1;
}
