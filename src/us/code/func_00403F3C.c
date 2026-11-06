#include "common.h"
#include "structs/boss.h"

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

extern f32 D_800B6D28;
extern u8 D_00431CD4[];
extern u8 D_00431CF0[];
extern u8 D_00431D0C[];
extern u8 D_00431D14[];
extern u8 D_801100F0[];
void AI_TurnHeadToTarget(Actor00403F3C *actor, Level00403F3C *level, s32 a2);
void Animator_Update(void *light);
s16 WeightedTable_PickStatic(void *table);
void Primagen_SpawnTossedObject(s16 type, s32 a1, Vec3_00403F3C offset);
void Primagen_UpdateHealth(Actor00403F3C *actor, Level00403F3C *level);
void Primagen_UpdateSpawner(Actor00403F3C *actor, Level00403F3C *level);
void COnScreen__SetBossPercent(void *a0, s32 percent);
void CAI__SetMode(Actor00403F3C *actor, Level00403F3C *level, s32 event);

#define COUNT_DOWN(t)              \
    if ((t) > 0.0f) {              \
        (t) -= D_800B6D28;         \
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
        Animator_Update(&D_8012F608.anims[i]);
    }
    if (D_8012F608.bossState == 1) {
        level->spawn.x = D_8012F608.home.x;
        level->spawn.y = D_8012F608.home.y;
        level->spawn.z = D_8012F608.home.z;
    }
    COUNT_DOWN(D_8012F608.waveTimer);
    COUNT_DOWN(D_8012F608.script.timer);
    COUNT_DOWN(D_8012F608.script2.timer);
    COUNT_DOWN(D_8012F608.script3.timer);
    COUNT_DOWN(D_8012F608.spawnTimer);
    COUNT_DOWN(D_8012F608.swayTimer);
    COUNT_DOWN(D_8012F608.nextTimer);
    COUNT_DOWN(D_8012F608.timer3EC);
    if (D_8012F608.stage == 3) {
        if (D_8012F608.values[0][1] <= 0x95FF && D_8012F608.subStage == 0) {
            D_8012F608.subStage = 1;
        }
        if (D_8012F608.values[0][1] < 0x6400 && D_8012F608.subStage == 1) {
            D_8012F608.subStage = 2;
        }
        if (D_8012F608.values[0][1] < 0x3200 && D_8012F608.subStage == 2) {
            D_8012F608.subStage = 3;
        }
    }
    if (D_8012F608.timer3EC <= 0.0f) {
        offset.x = -2385.92f;
        offset.y = 0.0f;
        offset.z = 0.0f;
        if (D_8012F608.stage == 3) {
            type = WeightedTable_PickStatic(D_00431CF0);
        } else {
            type = WeightedTable_PickStatic(D_00431CD4);
        }
        Primagen_SpawnTossedObject(type, 0x11D, offset);
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = -2324.48f;
        if (D_8012F608.stage == 3) {
            type = WeightedTable_PickStatic(D_00431CF0);
        } else {
            type = WeightedTable_PickStatic(D_00431CD4);
        }
        Primagen_SpawnTossedObject(type, 0x11D, offset);
        offset.x = 2314.24f;
        offset.y = 0.0f;
        offset.z = 0.0f;
        if (D_8012F608.stage == 3) {
            type = WeightedTable_PickStatic(D_00431D14);
        } else {
            type = WeightedTable_PickStatic(D_00431D0C);
        }
        Primagen_SpawnTossedObject(type, 0x11D, offset);
        offset.x = -460.8f;
        offset.y = 0.0f;
        offset.z = -460.8f;
        Primagen_SpawnTossedObject(WeightedTable_PickStatic(D_00431CD4), 0x11D, offset);
        offset.x = 460.8f;
        offset.y = 0.0f;
        offset.z = -460.8f;
        Primagen_SpawnTossedObject(WeightedTable_PickStatic(D_00431CD4), 0x11D, offset);
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 614.39996f;
        Primagen_SpawnTossedObject(WeightedTable_PickStatic(D_00431CD4), 0x11D, offset);
        D_8012F608.timer3EC = 300.0f;
    }
    Primagen_UpdateHealth(actor, level);
    Primagen_UpdateSpawner(actor, level);
    COnScreen__SetBossPercent(D_801100F0, D_8012F608.total * 100 / D_8012F608.bonus);
    if (actor->pos.y < -204.79999f) {
        D_8012F608.bossMode = 11;
        D_8012F608.dest.x = 0.0f;
        D_8012F608.dest.y = 0.0f;
        D_8012F608.dest.z = 0.0f;
        CAI__SetMode(actor, level, 0x19);
    }
}
