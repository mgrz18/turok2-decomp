#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/onscreen.h"
#include "globals/engine.h"
#include "protos/ai.h"

typedef struct {
    f32 x, y, z;
} Vec3_00403F3C;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00403F3C pos;
} Actor00403F3C;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ void (*onReset)(Actor00403F3C *actor, void *level);
} Def00403F3C;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ Def00403F3C *def;
    /* 0x34 */ u8 pad1[0x54 - 0x34];
    /* 0x54 */ Vec3_00403F3C spawn;
} Level00403F3C;

void AI_TurnHeadToTarget(Actor00403F3C *actor, Level00403F3C *level, s32 a2);
void Animator_Update(void *light);
s16 WeightedTable_PickStatic(void *table);
void Primagen_SpawnTossedObject(s16 type, s32 a1, Vec3_00403F3C offset);
void Primagen_UpdateHealth(Actor00403F3C *actor, Level00403F3C *level);
void Primagen_UpdateSpawner(Actor00403F3C *actor, Level00403F3C *level);
void COnScreen__SetBossPercent(void *a0, s32 percent);

#define COUNT_DOWN(t)              \
    if ((t) > 0.0f) {              \
        (t) -= gFrameIncrement;         \
    }

void Primagen_Step(Actor00403F3C *actor, Level00403F3C *level) {
    Vec3_00403F3C offset;
    s32 type;
    s32 i;

    AI_TurnHeadToTarget(actor, level, 0);
    if (level->def != NULL && level->def->onReset != NULL) {
        level->def->onReset(actor, level);
    }
    for (i = 0; i < 8; i++) {
        Animator_Update(&gLevelScript.anims[i]);
    }
    if (gLevelScript.bossState == 1) {
        level->spawn.x = gLevelScript.home.x;
        level->spawn.y = gLevelScript.home.y;
        level->spawn.z = gLevelScript.home.z;
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
    if (actor->pos.y < -204.79999f) {
        gLevelScript.bossMode = 11;
        gLevelScript.dest.x = 0.0f;
        gLevelScript.dest.y = 0.0f;
        gLevelScript.dest.z = 0.0f;
        CAI__SetMode(actor, level, 0x19);
    }
}
