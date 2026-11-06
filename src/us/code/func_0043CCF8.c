/* Second-module twin of Primagen_InitAI (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/scene.h"

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

extern s32 gLevelScript_path; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern u8 gScene[];
extern u8 sPrimagenModeTable_2[];
s32 CScene__FindInstances(void *world, s32 key0, s32 key1, s32 key2, void **out, s32 max);
void *CScene__FindIntelligence(void *world, s32 a1, s32 id);
void ScriptAnimator_Start(void *light, void *actor);
void Primagen_SetStage_2(s32 mode);
void Primagen_SetSpawnScript_2(s32 a0);
void Primagen_SetAttackScript_2(s32 a0);
void CAI__SetMode(void *actor, void *level, s32 event);
void Primagen_Step_2();
void Primagen_Hurt();
void Primagen_Effects();
void Primagen_Draw_2();

void Primagen_InitAI_2(Actor00403CF8 *actor, Level00403CF8 *level) {
    s32 i;

    gScene_m_LevelID = 0x1DB2;
    level->data = sPrimagenModeTable_2;
    level->onReset = Primagen_Step_2;
    level->onHit = Primagen_Hurt;
    level->onDraw = Primagen_Effects;
    level->onEvent = Primagen_Draw_2;
    level->unkC = -1;
    gLevelScript.actor = actor;
    CScene__FindInstances(gScene, -1, 6, 0xC8, &gLevelScript.found[1], 1);
    CScene__FindInstances(gScene, -1, 6, 0xC9, &gLevelScript.found[0], 1);
    CScene__FindInstances(gScene, -1, 6, 0xCA, &gLevelScript.found[2], 1);
    CScene__FindInstances(gScene, -1, 6, 0xCB, &gLevelScript.found[3], 1);
    CScene__FindInstances(gScene, -1, 6, 0xCC, &gLevelScript.found[4], 1);
    gLevelScript.intel = CScene__FindIntelligence(gScene, 1, 0x1DB2);
    for (i = 0; i < 8; i++) {
        ScriptAnimator_Start(&gLevelScript.anims[i], actor);
    }
    Primagen_SetStage_2(1);
    Primagen_SetSpawnScript_2(1);
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
    Primagen_SetAttackScript_2(1);
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
