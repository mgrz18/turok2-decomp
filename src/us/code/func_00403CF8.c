#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/scene.h"
#include "globals/boss_ai.h"
#include "protos/scene.h"
#include "protos/ai.h"

typedef struct {
    f32 x, y, z;
} Vec3_00403CF8;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00403CF8 pos;
} Actor00403CF8;

typedef struct {
    /* 0x000 */ u8 pad0[0xC];
    /* 0x00C */ s32 unkC;
    /* 0x010 */ u8 pad1[0x2C - 0x10];
    /* 0x02C */ void *data;
    /* 0x030 */ u8 pad2[0x37 - 0x30];
    /* 0x037 */ s8 unk37;
    /* 0x038 */ u8 pad3[0xF0 - 0x38];
    /* 0x0F0 */ void *onReset;
    /* 0x0F4 */ void *onEvent;
    /* 0x0F8 */ void *onHit;
    /* 0x0FC */ u8 pad4[0x108 - 0xFC];
    /* 0x108 */ void *onDraw;
} Level00403CF8;

void ScriptAnimator_Start(void *light, void *actor);
void Primagen_SetStage(s32 mode);
void Primagen_SetSpawnScript(s32 a0);
void Primagen_SetAttackScript(s32 a0);
void Primagen_Step();
void Primagen_Hurt();
void Primagen_Effects();
void Primagen_Draw();

void Primagen_InitAI(Actor00403CF8 *actor, Level00403CF8 *level) {
    s32 i;

    gScene_m_LevelID = 0x1DB2;
    level->data = sPrimagenModeTable;
    level->onReset = Primagen_Step;
    level->onHit = Primagen_Hurt;
    level->onDraw = Primagen_Effects;
    level->onEvent = Primagen_Draw;
    level->unkC = -1;
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
    gLevelScript.home.x = actor->pos.x;
    gLevelScript.home.y = actor->pos.y;
    gLevelScript.home.z = actor->pos.z;
    gLevelScript_path = 0;
    sPrimagenPathPoint = 0;
    CAI__SetMode(actor, level, 0);
    level->unk37 = 1;
}
