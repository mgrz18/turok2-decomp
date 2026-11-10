#include "common.h"
#include "structs/boss.h"
#include "structs/game_object.h"
#include "globals/boss.h"
#include "globals/scene.h"
#include "globals/boss_ai.h"
#include "protos/scene.h"
#include "protos/ai.h"

typedef struct {
    f32 x, y, z;
} Vec3_00403CF8;

#ifdef TARGET_PORT
typedef struct Actor00403CF8 {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Actor00403CF8(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_unk0[T2_HOFF_Actor00403CF8(header)]; s32 unk0; }; /* N64 0x0 */
        struct { u8 _hp_pos[T2_HOFF_Actor00403CF8(header.m_vPos)]; Vec3_00403CF8 pos; }; /* N64 0x4 */
    };
} Actor00403CF8;
#else
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00403CF8 pos;
} Actor00403CF8;
#endif

#ifdef TARGET_PORT
typedef struct Level00403CF8 {
/* the view at the host offsets of CAI (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Level00403CF8(m) __builtin_offsetof(CAI, m)
    union {
        struct { u8 _hp_unkC[T2_HOFF_Level00403CF8(m_VisBits)]; s32 unkC; }; /* N64 0xC */
        struct { u8 _hp_data[T2_HOFF_Level00403CF8(m_pModeTable)]; void *data; }; /* N64 0x2C */
        struct { u8 _hp_unk37[T2_HOFF_Level00403CF8(m_Action)]; s8 unk37; }; /* N64 0x37 */
        struct { u8 _hp_onReset[T2_HOFF_Level00403CF8(m_pfStep)]; void *onReset; }; /* N64 0xF0 */
        struct { u8 _hp_onEvent[T2_HOFF_Level00403CF8(m_pfDraw)]; void *onEvent; }; /* N64 0xF4 */
        struct { u8 _hp_onHit[T2_HOFF_Level00403CF8(m_pfHurt)]; void *onHit; }; /* N64 0xF8 */
        struct { u8 _hp_onDraw[T2_HOFF_Level00403CF8(m_pfEffects)]; void *onDraw; }; /* N64 0x108 */
    };
} Level00403CF8;
#else
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
#endif

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
