#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"

void Primagen_SetStage_2(s32 mode) {
    if (mode == 1) {
        gLevelScript.partsDone = 120;
        gLevelScript.values[0][3] = 0x1400;
        gLevelScript.values[0][4] = 0x1400;
        gLevelScript.values[0][6] = 0x1400;
        gLevelScript.values[0][5] = 0x1400;
        gLevelScript.values[1][3] = 0x1400;
        gLevelScript.values[1][4] = 0x1400;
        gLevelScript.values[1][6] = 0x1400;
        gLevelScript.values[1][5] = 0x1400;
        gLevelScript.values[0][2] = 0x3200;
        gLevelScript.values[0][0] = 0x5000;
        gLevelScript.values[1][2] = 0x3200;
        gLevelScript.values[1][0] = 0x5000;
        gLevelScript.values[0][1] = 0xC800;
        gLevelScript.values[1][1] = 0xC800;
        gLevelScript.spawnTimer = 30.0f;
    } else if (mode == 2) {
        gLevelScript.partsDone = 5;
        gLevelScript.values[0][2] = 0x3200;
        gLevelScript.values[0][0] = 0x5000;
        gLevelScript.values[1][2] = 0x3200;
        gLevelScript.values[1][0] = 0x5000;
    } else if (mode == 3) {
        gLevelScript.partsDone = 2;
        gLevelScript.values[0][1] = 0xC800;
        gLevelScript.values[1][1] = 0xC800;
    }
    gLevelScript.stage = mode;
    gLevelScript.scriptCOn = 0;
    gLevelScript.partActive = 0;
}

#include "structs/game_object.h"
#include "globals/scene.h"
#include "globals/boss_ai.h"
#include "protos/scene.h"
#include "protos/ai.h"
#include "structs/vector.h"

void ScriptAnimator_Start(void *light, void *actor);
void Primagen_SetSpawnScript_2(s32 arg0);
void Primagen_SetAttackScript_2(s32 arg0);
void Primagen_Step_2(CGameObjectInstance *actor, CAI *level);
void Primagen_Hurt();
void Primagen_Effects();
#include "structs/twin_primagen.h"
void Primagen_Draw_2(u8 *actor, u8 *state, CDrawArgs *req);

void Primagen_InitAI_2(CGameObjectInstance *actor, CAI *level) {
    s32 i;

    gScene_m_LevelID = 0x1DB2;
    level->m_pModeTable = sPrimagenModeTable_2;
    level->m_pfStep = Primagen_Step_2;
    level->m_pfHurt = Primagen_Hurt;
    level->m_pfEffects = Primagen_Effects;
    level->m_pfDraw = Primagen_Draw_2;
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
    gLevelScript.home.x = actor->header.m_vPos.x;
    gLevelScript.home.y = actor->header.m_vPos.y;
    gLevelScript.home.z = actor->header.m_vPos.z;
    gLevelScript_path = 0;
    sPrimagenPathPoint = 0;
    CAI__SetMode(actor, level, 0);
    level->m_Action = 1;
}

#include "globals/onscreen.h"
#include "globals/engine.h"
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define AI_TurnHeadToTarget AI_TurnHeadToTarget_i
#endif

void AI_TurnHeadToTarget(CGameObjectInstance *actor, CAI *level, s32 a2);
void Animator_Update(void *light);
s16 WeightedTable_PickStatic(void *table);
void Primagen_SpawnTossedObject_2(s32 kind, s32 count, CVector3 pos);
void Primagen_UpdateHealth_2(sptr a0, sptr a1);
void Primagen_UpdateSpawner_2(CGameObjectInstance *actor, void *level);
void COnScreen__SetBossPercent(void *a0, s32 percent);
#define COUNT_DOWN(t)              \
    if ((t) > 0.0f) {              \
        (t) -= gFrameIncrement;         \
    }

void Primagen_Step_2(CGameObjectInstance *actor, CAI *level) {
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
            type = WeightedTable_PickStatic(sPrimagenTossTableStage3_2);
        } else {
            type = WeightedTable_PickStatic(sPrimagenTossTable_2);
        }
        Primagen_SpawnTossedObject_2(type, 0x11D, offset);
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = -2324.48f;
        if (gLevelScript.stage == 3) {
            type = WeightedTable_PickStatic(sPrimagenTossTableStage3_2);
        } else {
            type = WeightedTable_PickStatic(sPrimagenTossTable_2);
        }
        Primagen_SpawnTossedObject_2(type, 0x11D, offset);
        offset.x = 2314.24f;
        offset.y = 0.0f;
        offset.z = 0.0f;
        if (gLevelScript.stage == 3) {
            type = WeightedTable_PickStatic(sPrimagenTossTable2Stage3_2);
        } else {
            type = WeightedTable_PickStatic(sPrimagenTossTable2_2);
        }
        Primagen_SpawnTossedObject_2(type, 0x11D, offset);
        offset.x = -460.8f;
        offset.y = 0.0f;
        offset.z = -460.8f;
        Primagen_SpawnTossedObject_2(WeightedTable_PickStatic(sPrimagenTossTable_2), 0x11D, offset);
        offset.x = 460.8f;
        offset.y = 0.0f;
        offset.z = -460.8f;
        Primagen_SpawnTossedObject_2(WeightedTable_PickStatic(sPrimagenTossTable_2), 0x11D, offset);
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 614.39996f;
        Primagen_SpawnTossedObject_2(WeightedTable_PickStatic(sPrimagenTossTable_2), 0x11D, offset);
        gLevelScript.timer3EC = 300.0f;
    }
    Primagen_UpdateHealth_2(actor, level);
    Primagen_UpdateSpawner_2(actor, level);
    COnScreen__SetBossPercent(&gEngineApp_m_OnScreen, gLevelScript.total * 100 / gLevelScript.bonus);
    if (actor->header.m_vPos.y < -204.79999f) {
        gLevelScript.bossMode = 11;
        gLevelScript.dest.x = 0.0f;
        gLevelScript.dest.y = 0.0f;
        gLevelScript.dest.z = 0.0f;
        CAI__SetMode(actor, level, 0x19);
    }
}

void Primagen_UpdateHealth_2(sptr a0, sptr a1) {
    s32 count;
    s32 i;
    s32 bit;

    count = 0;
    gLevelScript.bonus = 0;
    gLevelScript.total = 0;
    for (i = 0; i < 8; i++) {
        bit = 1 << i;
        if (gLevelScript.partsDone & bit) {
            gLevelScript.bonus += gLevelScript.values[1][i];
            if (gLevelScript.values[0][i] > 0) {
                gLevelScript.total += gLevelScript.values[0][i];
                count++;
            }
        }
    }
    if (gLevelScript.partActive == 0) {
        if (gLevelScript.stage == 3) {
            if (count == 0) {
                CAI__SetMode(a0, a1, 0x1D);
            }
        } else if (count == 0) {
            gLevelScript.stage++;
            if (gLevelScript.stage >= 2) {
                gLevelScript.bossMode = 3;
                gLevelScript.bossState = 3;
                gLevelScript.armed = 0;
                gLevelScript.scriptOn = 0;
                gLevelScript.dest.x = gLevelScript.home.x;
                gLevelScript.dest.y = gLevelScript.home.y;
                gLevelScript.dest.z = gLevelScript.home.z;
                CAI__SetMode(a0, a1, 0x12);
            }
            Primagen_SetStage_2(gLevelScript.stage);
        }
    }
}

#include "structs/game.h"
#include "globals/game.h"
#include "globals/collision.h"

extern ModeState0040457C gLevelScript_v0040457C __asm__("gLevelScript"); /* local type: globals/boss.h declares it; this view (its type here) is what keeps the ROM's code */
CGameObjectInstance *CPlayerPool__FindNearest(void *game, CVector3 pos);
f32 Random_Blend(f32 lo, f32 hi);
struct CGameObjectInstance *CInstancePool__Spawn(void *, s32, s32, void *, void *, CVector3, f32, CVector3, CVector3, s32 *);
void AI_UpdateTargets(CGameObjectInstance *, CAI *, CAITargets *, s32, s32);
void Primagen_NextSpawnStep_2(void);
s32 Primagen_RunSpawnStep_2(void);
/* the ROM's call passes the actor, the CAI and the target's word in a0-a2 (the definition reads none) */
#define Primagen_RunSpawnStep_3 ((void (*)(CGameObjectInstance *, void *, s32)) Primagen_RunSpawnStep_2)

void Primagen_NextSoundStep_2(void);
s32 Primagen_RunSoundStep_2(CGameObjectInstance *obj);
#define Primagen_RunSoundStep_3 ((void (*)(CGameObjectInstance *, void *, s32)) Primagen_RunSoundStep_2)

static __inline__ Wave0040457C *CurrentWave(void) {
    return (Wave0040457C *)(gLevelScript_v0040457C.wave * sizeof(Wave0040457C) + (sptr)gLevelScript_v0040457C.waves);
}

void Primagen_UpdateSpawner_2(CGameObjectInstance *actor, void *level) {
    Info0040457C info;
    CVector3 scale;
    CVector3 pos;
    s32 nextWave = 0;
    CGameObjectInstance *player;
    CListedInstance *enemy;
    Wave0040457C *wave;
    s32 extra;
    f32 delay;
    s32 *flags;

    player = CPlayerPool__FindNearest(&gEngineApp_m_Game.m_PlayerList, actor->header.m_vPos);
    if (player == NULL) {
        return;
    }
    if (gLevelScript_v0040457C.resetRegion != 0) {
        gLevelScript_v0040457C.region = player->header.m_pCurrentRegion;
        gLevelScript_v0040457C.resetRegion = 0;
    }
    if (!gLevelScript_v0040457C.doorsOpen) {
        if (gLevelScript_v0040457C.spawnsLeft > 0) {
            gLevelScript_v0040457C.doorsOpen = 1;
            CAI__SetMode(gLevelScript_v0040457C.found[2], gLevelScript_v0040457C.found[2]->ai, 0);
            CAI__SetMode(gLevelScript_v0040457C.found[3], gLevelScript_v0040457C.found[3]->ai, 0);
        }
    }
    if (gLevelScript_v0040457C.doorsOpen) {
        if (gLevelScript_v0040457C.spawnsLeft == 0 && gLevelScript_v0040457C.spawnTimer <= 0.0f) {
            gLevelScript_v0040457C.doorsOpen = 0;
            CAI__SetMode(gLevelScript_v0040457C.found[2], gLevelScript_v0040457C.found[2]->ai, 1);
            CAI__SetMode(gLevelScript_v0040457C.found[3], gLevelScript_v0040457C.found[3]->ai, 1);
        }
    }
    if (gLevelScript_v0040457C.wavesLeft > 0 && gLevelScript_v0040457C.nextTimer <= 0.0f && gLevelScript_v0040457C.wave != -1) {
        delay = CurrentWave()->delay;
        extra = CurrentWave()->count;
        gLevelScript_v0040457C.wavesLeft--;
        if (gLevelScript_v0040457C.wavesLeft == 0) {
            gLevelScript_v0040457C.wavesDone = 1;
            gLevelScript_v0040457C.wave = -1;
        } else if (extra != 3) {
            extra -= 3;
            gLevelScript_v0040457C.nextTimer = delay - extra * 15 + Random_Blend(0.0f, extra * 2) * 15.0f;
        } else {
            gLevelScript_v0040457C.nextTimer = delay;
        }
        CAI__SetMode(gLevelScript_v0040457C.found[4], gLevelScript_v0040457C.found[4]->ai, 0);
    }
    if ((u32)(gLevelScript_v0040457C.spawnsLeft - 1) < 8 && gLevelScript_v0040457C.waveTimer <= 0.0f) {
        pos.x = 1669.12f;
        pos.y = 1064.96f;
        pos.z = 2816.0f;
        scale.x = 2.0f;
        scale.y = 2.0f;
        scale.z = 2.0f;
        if (CInstancePool__Spawn(&gScene.m_InstancePool, -1, 0x387, (void *) gLevelScript_v0040457C.handle, (void *) gLevelScript_v0040457C.region, pos, 0.0f, actor->m_vVelocity,
                          scale, NULL)) {
            gLevelScript_v0040457C.spawnsLeft--;
            if (gLevelScript_v0040457C.waves != NULL) {
                if (gLevelScript_v0040457C.spawnsLeft == 0 && CurrentWave()->type != 1) {
                    gLevelScript_v0040457C.wavesDone = 1;
                    gLevelScript_v0040457C.wave = -1;
                } else {
                    nextWave = 1;
                }
            }
            gLevelScript_v0040457C.spawnTimer = 120.0f;
        }
    }
    if ((u32)(gLevelScript_v0040457C.spawnsLeft - 1) < 8 && gLevelScript_v0040457C.waveTimer <= 0.0f) {
        pos.x = -1699.84f;
        pos.y = 1064.96f;
        pos.z = 2877.44f;
        scale.x = 2.0f;
        scale.y = 2.0f;
        scale.z = 2.0f;
        if (CInstancePool__Spawn(&gScene.m_InstancePool, -1, 0x387, (void *) gLevelScript_v0040457C.handle, (void *) gLevelScript_v0040457C.region, pos, 0.0f, actor->m_vVelocity,
                          scale, NULL)) {
            gLevelScript_v0040457C.spawnsLeft--;
            if (gLevelScript_v0040457C.waves != NULL) {
                if (gLevelScript_v0040457C.spawnsLeft == 0 && CurrentWave()->type != 1) {
                    gLevelScript_v0040457C.wavesDone = 1;
                    gLevelScript_v0040457C.wave = -1;
                } else {
                    nextWave = 1;
                }
            }
        }
    }
    if (nextWave && gLevelScript_v0040457C.wave != -1) {
        delay = CurrentWave()->delay;
        extra = CurrentWave()->count;
        if (gLevelScript_v0040457C.waves != NULL) {
            if (extra != 3) {
                extra -= 3;
                gLevelScript_v0040457C.waveTimer = delay - extra * 15 + Random_Blend(0.0f, extra * 2) * 15.0f;
            } else {
                gLevelScript_v0040457C.waveTimer = delay;
            }
        }
    }
    gLevelScript_v0040457C.enemiesAlive = 0;
    for (enemy = gScene.m_InstancePool.m_ActiveList.m_pHead; enemy != NULL; enemy = enemy->m_pNext) {
        enemy->m_Instance.m_AI.m_vLeashPos.x = 0;
        enemy->m_Instance.m_AI.m_vLeashPos.y = 768.0f;
        enemy->m_Instance.m_AI.m_vLeashPos.z = 0;
        enemy->m_Instance.m_AI.m_pLeashRegion = (struct CGameRegion *)gLevelScript_v0040457C.region;
        if (enemy->m_Instance.m_AI.m_Health > 0) {
            gLevelScript_v0040457C.enemiesAlive++;
        }
    }
    flags = &gCollisionInfoFlyingEnemy.m_dwFlags;
    *flags |= 0x8000;
    AI_UpdateTargets(actor, level, (CAITargets *) &info, 0, 1);
    *flags &= ~0x8000;
    if (gLevelScript_v0040457C.wave == -1) {
        Primagen_NextSpawnStep_2();
    }
    Primagen_RunSpawnStep_3(actor, level, info.unk24);
    if (gLevelScript_v0040457C.unk420 == -1) {
        Primagen_NextSoundStep_2();
    }
    Primagen_RunSoundStep_3(actor, level, info.unk24);
    if (gLevelScript_v0040457C.waves != NULL) {
        if (gLevelScript_v0040457C.enemiesAlive == 0) {
            wave = CurrentWave();
            if (wave->type == 1 && wave->flag == 1 && gLevelScript_v0040457C.spawnsLeft == 0 && gLevelScript_v0040457C.unk400 <= 0.0f) {
                gLevelScript_v0040457C.wavesDone = 1;
                gLevelScript_v0040457C.wave = -1;
            }
        }
        if (gLevelScript_v0040457C.waves != NULL) {
            return;
        }
    }
    if (gLevelScript_v0040457C.unk3D0 == 1) {
        gLevelScript_v0040457C.unk3D4 = 0;
    }
}

#define BOSS_STAGE_AS_WORD

extern s32 gLevelScript_v00404C44 __asm__("gLevelScript"); /* local type: globals/boss.h declares it; this view (its type here) is what keeps the ROM's code */           /* stage */
extern f32 Random_Blend(f32 lo, f32 hi);
extern BossWait gLevelScript_swayPhase; /* local type: the field's real type (or the member path) changes the ROM's code here */

s32 Primagen_RunAttackStep_2(CGameObjectInstance *obj, CAI *ai, f32 dist) {
    f32 scale;

    if (gLevelScript_bossMode.script.timer > 0.0f) {
        return 0;
    }
    switch (gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].type) {
        case 2:
            if (gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].arg == 1) {
                gLevelScript_swayPhase.waitMax = 2.5f;
                gLevelScript_swayPhase.waitMin = 0.0f;
            }
            if (gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].arg == 2) {
                gLevelScript_swayPhase.waitMin = Random_Blend(0.0f, 2.0f);
                gLevelScript_swayPhase.waitMax = Random_Blend(3.0f, 4.5f);
            }
            if (gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].arg == 3) {
                gLevelScript_swayPhase.waitMax = 4.0f;
                gLevelScript_swayPhase.waitMin = 1.0f;
            }
            gLevelScript_bossMode.mode = 9;
            CAI__SetMode(obj, ai, 0x12);
            return 1;
        case 3:
            scale = 10.24f;
            if (sPrimagenAttacks_2[gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].arg].minDist * scale <= dist
                && dist <= sPrimagenAttacks_2[gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].arg].maxDist * scale) {
                obj->m_AI.m_AnimSpeed = gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].animSpeed;
                CAI__SetMode(obj, ai, sPrimagenAttacks_2[gLevelScript_bossMode.script.steps[gLevelScript_bossMode.script.current].arg].mode);
                return 1;
            }
            break;
        case 4:
        case 5:
            break;
        case 6:
            if (gLevelScript_v00404C44 == 1) {
                gLevelScript_bossMode.script.steps = sPrimagenAttackScript2Stage1_2;
            }
            if (gLevelScript_v00404C44 == 2) {
                gLevelScript_bossMode.script.steps = sPrimagenAttackScript2Stage2_2;
            }
            if (gLevelScript_v00404C44 == 3) {
                gLevelScript_bossMode.script.steps = sPrimagenAttackScript2Stage3_2;
            }
            gLevelScript_bossMode.script.cursor = -1;
            gLevelScript_bossMode.script.current = -1;
            gLevelScript_bossMode.mode = 4;
            CAI__SetMode(obj, ai, 0x12);
            return 1;
        case 7:
            if (gLevelScript_v00404C44 == 1) {
                gLevelScript_bossMode.script.steps = sPrimagenAttackScriptStage1_2;
            }
            if (gLevelScript_v00404C44 == 2) {
                gLevelScript_bossMode.script.steps = sPrimagenAttackScriptStage2_2;
            }
            if (gLevelScript_v00404C44 == 3) {
                gLevelScript_bossMode.script.steps = sPrimagenAttackScriptStage3_2;
            }
            gLevelScript_bossMode.script.cursor = -1;
            gLevelScript_bossMode.script.current = -1;
            gLevelScript_bossMode.mode = 2;
            CAI__SetMode(obj, ai, 0x18);
            return 1;
    }
    return 0;
}

extern LevelScriptF950 gLevelScript_spawnsLeft_v00404FE0 __asm__("gLevelScript_spawnsLeft"); /* local type: the field's real type (or the member path) changes the ROM's code here */

/* Advance the second script to its next step, skipping steps by chance. */
void Primagen_NextSpawnStep_2(void) {
    if (gLevelScript_spawnsLeft_v00404FE0.script2.steps == NULL) {
        return;
    }
    if (gLevelScript_spawnsLeft_v00404FE0.script2.cursor != -1) {
        gLevelScript_spawnsLeft_v00404FE0.script2.timer = gLevelScript_spawnsLeft_v00404FE0.script2.steps[gLevelScript_spawnsLeft_v00404FE0.script2.cursor].delay;
    } else {
        gLevelScript_spawnsLeft_v00404FE0.script2.timer = 0.0f;
    }
    do {
        gLevelScript_spawnsLeft_v00404FE0.script2.cursor++;
        if (gLevelScript_spawnsLeft_v00404FE0.script2.steps[gLevelScript_spawnsLeft_v00404FE0.script2.cursor].type == 4) {
            gLevelScript_spawnsLeft_v00404FE0.script2On = 0;
            gLevelScript_spawnsLeft_v00404FE0.script2.cursor = -1;
            gLevelScript_spawnsLeft_v00404FE0.script2.current = -1;
            gLevelScript_spawnsLeft_v00404FE0.script2.steps = NULL;
            return;
        }
        gLevelScript_spawnsLeft_v00404FE0.script2.current = gLevelScript_spawnsLeft_v00404FE0.script2.cursor;
    } while ((f32) gLevelScript_spawnsLeft_v00404FE0.script2.steps[gLevelScript_spawnsLeft_v00404FE0.script2.cursor].chance <= Random_Blend(0.0f, 100.0f));
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))

extern LevelScriptF950 gLevelScript_spawnsLeft_v004050E0 __asm__("gLevelScript_spawnsLeft"); /* local type: the field's real type (or the member path) changes the ROM's code here */

s32 Primagen_RunSpawnStep_2(void) {
    if (gLevelScript_spawnsLeft_v004050E0.script2.timer > 0.0f) {
        return 0;
    }
    if (!gLevelScript_spawnsLeft_v004050E0.script2On) {
        return 0;
    }
    if (gLevelScript_spawnsLeft_v004050E0.script2.steps == NULL) {
        return 0;
    }
    switch (gLevelScript_spawnsLeft_v004050E0.script2.steps[gLevelScript_spawnsLeft_v004050E0.script2.current].type) {
        case 1:
            if (gLevelScript_spawnsLeft_v004050E0.enemiesAlive < 9) {
                gLevelScript_spawnsLeft_v004050E0.spawnsLeft = MIN(gLevelScript_spawnsLeft_v004050E0.script2.steps[gLevelScript_spawnsLeft_v004050E0.script2.current].arg, 8 - gLevelScript_spawnsLeft_v004050E0.enemiesAlive);
            }
            gLevelScript_spawnsLeft_v004050E0.script2On = 0;
            return 1;
        case 2:
            gLevelScript_spawnsLeft_v004050E0.wavesLeft = gLevelScript_spawnsLeft_v004050E0.script2.steps[gLevelScript_spawnsLeft_v004050E0.script2.current].arg;
            gLevelScript_spawnsLeft_v004050E0.script2On = 0;
            return 1;
        case 5:
            gLevelScript_spawnsLeft_v004050E0.script2.current = -1;
            return 1;
        case 3:
            if (Random_Blend(0.0f, 100.0f) < 50.0f) {
                if (gLevelScript_spawnsLeft_v004050E0.enemiesAlive < 9) {
                    gLevelScript_spawnsLeft_v004050E0.spawnsLeft = MIN(gLevelScript_spawnsLeft_v004050E0.script2.steps[gLevelScript_spawnsLeft_v004050E0.script2.current].arg, 8 - gLevelScript_spawnsLeft_v004050E0.enemiesAlive);
                }
            } else {
                gLevelScript_spawnsLeft_v004050E0.wavesLeft = gLevelScript_spawnsLeft_v004050E0.script2.steps[gLevelScript_spawnsLeft_v004050E0.script2.current].arg;
            }
            gLevelScript_spawnsLeft_v004050E0.script2On = 0;
            return 1;
        case 4:
            break;
    }
    return 0;
}

/* Advance the third script to its next step, skipping steps by chance. */
void Primagen_NextSoundStep_2(void) {
    if (gLevelScript.script3.steps == NULL) {
        return;
    }
    if (gLevelScript.script3.cursor != -1) {
        gLevelScript.script3.timer = gLevelScript.script3.steps[gLevelScript.script3.cursor].delay
                         + Random_Blend(0.0f, gLevelScript.script3.steps[gLevelScript.script3.cursor].delayRange) * 15.0f;
    } else {
        gLevelScript.script3.timer = 0.0f;
    }
    do {
        gLevelScript.script3.cursor++;
        if (gLevelScript.script3.steps[gLevelScript.script3.cursor].type == 4) {
            gLevelScript.script3.cursor = -1;
            gLevelScript.script3.current = -1;
            gLevelScript.script3.steps = NULL;
            return;
        }
        gLevelScript.script3.current = gLevelScript.script3.cursor;
    } while ((f32) gLevelScript.script3.steps[gLevelScript.script3.cursor].chance <= Random_Blend(0.0f, 100.0f));
}

s16 WeightedTable_PickStatic(void *); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */
extern void Sound__Play(s16 sound, CVector3 pos, sptr arg2, CGameObjectInstance *source);
#ifdef TARGET_PORT
/* A step's arg is an N64 data address for steps 2 (a sound table) and 3 (the next script):
 * BossStep3.arg is an s32 in the data, so gendata keeps the N64 word; the host pointer comes
 * from the data symbol table (port/gen/relocs.c). */
void *t2_n64_to_host(u32 addr);
#define BOSS_STEP_PTR(v) ((sptr) t2_n64_to_host((u32) (v)))
/* the sound pick tables the type-2 steps name (round 10 y2): only an s32 in the data reaches them,
 * so nothing emitted them and t2_n64_to_host returned NULL (the first type-2 step crashed in
 * WeightedTable_PickStatic once the Primagen left its first stage). Referenced here, gendata
 * emits them as Pick tables (host byte order) and the symbol table maps their N64 addresses. */
#include "structs/pick.h"
Pick *const t2_sPrimagenSoundPicks[] = { sPrimagenSoundScript1Picks, sPrimagenSoundScript2Picks, sPrimagenSoundScript3Picks, sPrimagenSoundScript4Picks };
#else
#define BOSS_STEP_PTR(v) (v)
#endif

/* Run the current step of the third script (sounds, jumps, waits). */
s32 Primagen_RunSoundStep_2(CGameObjectInstance *obj) {
    s32 sound;
    s32 i;

    if (gLevelScript.script3.timer > 0.0f) {
        return 0;
    }
    if (gLevelScript.script3.steps == NULL) {
        return 0;
    }
    switch (gLevelScript.script3.steps[gLevelScript.script3.current].type) {
        case 1:
            if (gLevelScript.script3.steps[gLevelScript.script3.current].arg != -1) {
                Sound__Play(gLevelScript.script3.steps[gLevelScript.script3.current].arg, obj->header.m_vPos, 0, obj);
            }
            gLevelScript.script3.current = -1;
            return 1;
        case 2:
            sound = (s16) WeightedTable_PickStatic((void *) BOSS_STEP_PTR(gLevelScript.script3.steps[gLevelScript.script3.current].arg));
            if (sound != -1) {
                Sound__Play(sound, obj->header.m_vPos, 0, obj);
            }
            gLevelScript.script3.current = -1;
            return 1;
        case 3:
            gLevelScript.script3.steps = (BossStep3 *) BOSS_STEP_PTR(gLevelScript.script3.steps[gLevelScript.script3.current].arg);
            gLevelScript.script3.cursor = -1;
            gLevelScript.script3.current = -1;
            return 1;
        case 5:
            gLevelScript.script3.current = -1;
            return 1;
        case 6:
            gLevelScript.script3.timer = gLevelScript.script3.steps[gLevelScript.script3.cursor].delay
                             + Random_Blend(0.0f, gLevelScript.script3.steps[gLevelScript.script3.cursor].delayRange) * 15.0f;
            for (i = 0; i < gLevelScript.script3.steps[gLevelScript.script3.current].arg; i++) {
                gLevelScript.script3.cursor++;
                if (gLevelScript.script3.steps[gLevelScript.script3.cursor].type == 4 && i < gLevelScript.script3.steps[gLevelScript.script3.current].arg - 1) {
                    gLevelScript.script3.cursor = 0;
                }
                gLevelScript.script3.current = gLevelScript.script3.cursor;
            }
            if (gLevelScript.script3.steps[gLevelScript.script3.cursor].type == 4) {
                gLevelScript.script3.cursor = -1;
                gLevelScript.script3.current = -1;
                gLevelScript.script3.steps = NULL;
                return 1;
            }
            return 0;
        case 4:
            break;
    }
    return 0;
}

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);

void Primagen_UpdateMode1_2(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    gCollisionInfoFlyingEnemy.m_dwFlags |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x8000;

    if (gLevelScript_bossState.state == 10 && ai->m_AnimRepeatedCount != 0 && t.m_Sight.m_pTarget != 0) {
        gLevelScript_bossState.mode = 2;
        CAI__SetMode(obj, ai, 7);
        return;
    }
    if (gLevelScript_bossState.state == 5 && ai->m_AnimRepeatedCount != 0 && gLevelScript_bossState.timer <= 0.0f) {
        gLevelScript_bossState.mode = 1;
        CAI__SetMode(obj, ai, 5);
        return;
    }
    if (gLevelScript_bossState.timer > 0.0f) {
        gLevelScript_bossState.timer -= gFrameIncrement;
    }
    if (gLevelScript_bossState.scriptOn && gLevelScript_bossState.state != 10) {
        if (gLevelScript_bossState.script.current == -1) {
            if (gLevelScript_script_cursor != -1) {
                gLevelScript_script.timer = gLevelScript.script.steps[gLevelScript_script_cursor].delay;
            } else {
                gLevelScript_script.timer = 0.0f;
            }
            do {
                gLevelScript_script_cursor++;
                if (gLevelScript.script.steps[gLevelScript_script_cursor].type == 5) {
                    gLevelScript_script_cursor = 0;
                }
                gLevelScript_bossState.script.current = gLevelScript_script_cursor;
            } while ((f32) gLevelScript.script.steps[gLevelScript_script_cursor].chance <= Random_Blend(0.0f, 100.0f));
        }
        if (Primagen_RunAttackStep_2(obj, ai, t.m_Sight.m_Dist)) {
            return;
        }
    }
    if (gLevelScript_bossState.state == 6 && t.m_Sight.m_Dist > 460.8f) {
        gLevelScript_bossState.mode = 2;
        CAI__SetMode(obj, ai, 7);
    }
}

extern void AI_StopSound(CGameObjectInstance *obj, CAI *ai);

void Primagen_UpdateMode2_2(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;
    s32 limit;
    u32 bit;

    gCollisionInfoFlyingEnemy.m_dwFlags |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x8000;

    limit = 0x4B00;
    if (gLevelScript.stage == 0) {
        limit = 0x2580;
    }
    if (gLevelScript.damage >= limit) {
        AI_StopSound(obj, ai);
        ai->m_VisBits &= ~(1 << gLevelScript.part);
        gLevelScript.partActive = 0;
        CAI__SetMode(obj, ai, 7);
    } else if (gLevelScript.partTimer <= 0.0f) {
        AI_StopSound(obj, ai);
        gLevelScript.values[0][gLevelScript.part] = gLevelScript.values[1][gLevelScript.part];
        bit = 1 << gLevelScript.part;
        gLevelScript.partActive = 0;
        gLevelScript.partsDone |= bit;
        ai->m_VisBits |= bit;
        CAI__SetMode(obj, ai, 7);
    } else {
        gLevelScript.partTimer -= gFrameIncrement;
    }
}

extern f32 AI_AngleToPoint(CGameObjectInstance *obj, CVector3 point);       /* angle to a point */
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);       /* out = a - b */
extern f32 CVector3__GetYaw(CVector3 *v);                                    /* heading */
extern f32 Math_StepClamped(f32 value, f32 target, f32 rate);
extern f32 func_002982F0(f32 angle);                                      /* sin */
extern f32 func_002974C0(f32 angle);                                      /* cos */
extern s32 gLevelScript_script_current_v00405AC8[] __asm__("gLevelScript_script_current"); /* local type: the field's real type (or the member path) changes the ROM's code here */        /* boss script index */

void Primagen_UpdateMode27_2(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;
    CVector3 pos;
    CVector3 dir;
    f32 speed;
    f32 heading;
    f32 target;

    if (ai->m_AnimRepeatedCount != 0) {
        obj->m_AI.m_AnimSpeed = 1.0f;
        gLevelScript_script_current_v00405AC8[0] = -1;
        CAI__SetMode(obj, ai, 0x13);
    }
    gCollisionInfoFlyingEnemy.m_dwFlags |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x8000;
    if (t.m_Sight.m_pTarget == NULL) {
        return;
    }
    pos = t.m_Sight.m_pTarget->header.m_vPos;
    speed = 0.0f;
    AI_TurnClamped(obj, ai, 0.261799425f, AI_AngleToPoint(obj, pos));
    CVector3__Subtract(&dir, &obj->header.m_vPos, &pos);
    heading = CVector3__GetYaw(&dir);
    if (obj->m_CurrentAnimation.m_AnimationFrame <= 9.0f) {
        target = t.m_Sight.m_Dist * 0.229568392f;
    } else {
        target = speed;
    }
    speed = Math_StepClamped(speed, target, 512.0f);
    obj->m_vVelocity.x += speed * func_002982F0(heading);
    obj->m_vVelocity.z += speed * func_002974C0(heading);
    obj->m_vVelocity.y = 0.001f;
}

void ScriptAnimator_SetScript(void *, void *);
#ifdef TARGET_PORT
#define MODULE (&gLevelScript) /* the N64 delta 0x3D4 is not the host's */
#else
#define MODULE ((LevelScriptState *) ((u8 *) &gLevelScript_bossMode - 0x3D4))
#endif

void Primagen_EnterMode7_2(CGameObjectInstance *arg0) {
    LevelScriptState *temp_s2;

    if (gLevelScript_bossMode.mode == 2) {
        gLevelScript_bossMode.armed = 1;
        gLevelScript_bossMode.scriptOn = 1;
        MODULE->bossState = 2;
    }
    temp_s2 = MODULE;
    if (MODULE->stage == 1) {
        arg0->m_AI.m_AnimSpeed = 1.0f;
    }
    if (MODULE->stage == 2) {
        arg0->m_AI.m_AnimSpeed = 1.2f;
    }
    if (MODULE->stage == 3) {
        arg0->m_AI.m_AnimSpeed = 1.3f;
    }
    if ((MODULE->stage == 1) && (temp_s2->spawnStep == 0)) {
        ScriptAnimator_SetScript(&MODULE->anims[3], sPrimagenMode7PartScript_2);
        ScriptAnimator_SetScript(&MODULE->anims[4], sPrimagenMode7PartScript_2);
        ScriptAnimator_SetScript(&MODULE->anims[6], sPrimagenMode7PartScript_2);
        ScriptAnimator_SetScript(&MODULE->anims[5], sPrimagenMode7PartScript_2);
        temp_s2->spawnStep = (s32) (temp_s2->spawnStep + 1);
    }
    if ((gLevelScript.stage == 2) && (gLevelScript.spawnStep == 1)) {
        ScriptAnimator_SetScript(&gLevelScript.anims[0], sPrimagenMode7PartScript_2);
        ScriptAnimator_SetScript(&gLevelScript.anims[2], sPrimagenMode7PartScript_2);
        gLevelScript.spawnStep = (s32) (gLevelScript.spawnStep + 1);
    }
    if ((gLevelScript.stage == 3) && (gLevelScript.spawnStep == 2)) {
        ScriptAnimator_SetScript(&gLevelScript.anims[1], sPrimagenMode7PartScript_2);
        gLevelScript.spawnStep = (s32) (gLevelScript.spawnStep + 1);
    }
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
}

void Primagen_UpdateMode7_2(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    gCollisionInfoFlyingEnemy.m_dwFlags |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x8000;

    if (t.m_Sight.m_pTarget == NULL) {
        obj->m_AI.m_AnimSpeed = 1.0f;
        gLevelScript_bossState.mode = 10;
        CAI__SetMode(obj, ai, 1);
        return;
    }
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (!gLevelScript_bossState.scriptOn) {
        return;
    }
    if (gLevelScript_bossState.script.current == -1) {
        if (gLevelScript_script_cursor != -1) {
            gLevelScript_script.timer = gLevelScript.script.steps[gLevelScript_script_cursor].delay;
        } else {
            gLevelScript_script.timer = 0.0f;
        }
        do {
            gLevelScript_script_cursor++;
            if (gLevelScript.script.steps[gLevelScript_script_cursor].type == 5) {
                gLevelScript_script_cursor = 0;
            }
            gLevelScript_bossState.script.current = gLevelScript_script_cursor;
        } while ((f32) gLevelScript.script.steps[gLevelScript_script_cursor].chance <= Random_Blend(0.0f, 100.0f));
    }
    if (Primagen_RunAttackStep_2(obj, ai, t.m_Sight.m_Dist)) {
        return;
    }
    if (t.m_Sight.m_Dist <= 460.8f) {
        obj->m_AI.m_AnimSpeed = 1.0f;
        CAI__SetMode(obj, ai, 1);
    }
}

extern f32 func_00298470(f32 x);                                          /* sqrt */

extern s32 gLevelScript_v00406000 __asm__("gLevelScript"); /* local type: globals/boss.h declares it; this view (its type here) is what keeps the ROM's code */          /* stage, as the script selection reads it */

void Primagen_UpdateMode19_2(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;
    CVector3 d;
    CVector3 dest;
    CGameObjectInstance *target;
    f32 angle;
    f32 speed;
    f32 heading;
    BossCtl *ctl = (BossCtl *) &gLevelScript_unk3CC;  /* unk3CC on */

    gCollisionInfoFlyingEnemy.m_dwFlags |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 0, 1);
    ctl->unk0 = 3;
    gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x8000;
    do {
        target = t.m_Sight.m_pTarget;
        angle = 0.0f;
    } while (0);
    if (target != NULL) {
        if (ctl->state == 4) {
            dest = target->header.m_vPos;
            angle = AI_AngleToPoint(obj, dest);
        }
        dest = ctl->dest;
        if (ctl->state == 3 || ctl->state == 11) {
            angle = AI_AngleToPoint(obj, dest);
        }
        CVector3__Subtract(&d, &obj->header.m_vPos, &dest);
        if (func_00298470(d.x * d.x + d.z * d.z) <= 50.0f) {
            if (gLevelScriptBlock.m_Script.bossState == 3) {
                if (gLevelScript_v00406000 == 1) {
                    gLevelScriptBlock.m_Script.script.steps = sPrimagenAttackScriptStage1_2;
                }
                if (gLevelScript_v00406000 == 2) {
                    gLevelScriptBlock.m_Script.script.steps = sPrimagenAttackScriptStage2_2;
                }
                if (gLevelScript_v00406000 == 3) {
                    gLevelScriptBlock.m_Script.script.steps = sPrimagenAttackScriptStage3_2;
                }
                gLevelScriptBlock.m_Script.script.cursor = -1;
                gLevelScriptBlock.m_Script.script.current = -1;
                CAI__SetMode(gLevelScriptBlock.m_Script.found[1], &gLevelScriptBlock.m_Script.found[1]->m_AI, 0);
                CAI__SetMode(gLevelScriptBlock.m_Script.found[0], &gLevelScriptBlock.m_Script.found[0]->m_AI, 0);
                CAI__SetMode(obj, ai, 0x1A);
                return;
            }
            if (gLevelScriptBlock.m_Script.bossState == 11) {
                if (gLevelScript_v00406000 == 1) {
                    gLevelScriptBlock.m_Script.script.steps = sPrimagenAttackScriptStage1_2;
                }
                if (gLevelScript_v00406000 == 2) {
                    gLevelScriptBlock.m_Script.script.steps = sPrimagenAttackScriptStage2_2;
                }
                if (gLevelScript_v00406000 == 3) {
                    gLevelScriptBlock.m_Script.script.steps = sPrimagenAttackScriptStage3_2;
                }
                gLevelScriptBlock.m_Script.script.cursor = -1;
                gLevelScriptBlock.m_Script.script.current = -1;
                CAI__SetMode(obj, ai, 0x17);
                return;
            }
            if (gLevelScriptBlock.m_Script.bossState == 4) {
                sPrimagenPathPoint++;
                if (gPrimagenPaths[gLevelScript_path][sPrimagenPathPoint].y < 0.0f) {
                    sPrimagenPathPoint = 0;
                    if (gLevelScriptBlock.m_Script.stage != 1) {
                        gLevelScript_path++;
                        if (gLevelScript_path >= 3) {
                            gLevelScript_path = 0;
                        }
                    }
                }
                gLevelScriptBlock.m_Script.dest = gPrimagenPaths[gLevelScript_path][sPrimagenPathPoint];
            }
        }
    }
    speed = 0.0f;
    AI_TurnClamped(obj, ai, 0.261799425f, angle);
    dest = gLevelScriptBlock.m_Script.dest;
    CVector3__Subtract(&d, &obj->header.m_vPos, &dest);
    heading = CVector3__GetYaw(&d);
    if (gLevelScriptBlock.m_Script.bossState == 3 || gLevelScriptBlock.m_Script.bossState == 11) {
        if (obj->header.m_vPos.y < 921.6f) {
            obj->m_vVelocity.y = 460.8f;
        } else {
            obj->m_vVelocity.y = 0.001f;
        }
        speed = 500.0f;
        obj->m_vVelocity.x = func_002982F0(heading) * speed;
        obj->m_vVelocity.z = func_002974C0(heading) * speed;
    } else {
        speed = Math_StepClamped(speed, gLevelScriptBlock.m_Script.speed, 5120.0f);
        obj->m_vVelocity.x += speed * func_002982F0(heading);
        obj->m_vVelocity.z += speed * func_002974C0(heading);
        obj->m_vVelocity.y = 0.001f;
    }
    if (gLevelScriptBlock.m_Script.scriptOn && gLevelScriptBlock.m_Script.bossState != 3 && gLevelScriptBlock.m_Script.bossState != 11) {
        if (gLevelScriptBlock.m_Script.script.current == -1) {
            if (gLevelScript_script_cursor != -1) {
                gLevelScript_script.timer = gLevelScript_script_steps[gLevelScript_script_cursor].delay;
            } else {
                gLevelScript_script.timer = 0.0f;
            }
            do {
                gLevelScript_script_cursor++;
                if (gLevelScript_script_steps[gLevelScript_script_cursor].type == 5) {
                    gLevelScript_script_cursor = 0;
                }
                gLevelScriptBlock.m_Script.script.current = gLevelScript_script_cursor;
            } while ((f32) gLevelScript_script_steps[gLevelScript_script_cursor].chance <= Random_Blend(0.0f, 100.0f));
        }
        Primagen_RunAttackStep_2(obj, ai, t.m_Sight.m_Dist);
    }
}

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

void Primagen_UpdateMode18_2(CGameObjectInstance *obj, CAI *ai) {
    CVector3 point;
    f32 angle;

    if (gLevelScript.timer > 0.0f) {
        gLevelScript.timer -= gFrameIncrement;
    }
    if ((gLevelScript.bossMode == 3 || gLevelScript.bossMode == 4) && gLevelScript.timer <= 0.0f && ai->m_AnimRepeatedCount != 0) {
        obj->m_vVelocity.y = 0.0f;
        if (gLevelScript.bossState == 3) {
            gCollisionInfoEnemyB.m_dwFlags |= 0x300;
            gCollisionInfoFlyingEnemy.m_dwFlags |= 0x300;
            CAI__SetMode(obj, ai, 0x19);
        } else {
            CAI__SetMode(obj, ai, 0x13);
        }
        return;
    }
    if (gLevelScript.bossMode == 8) {
        point = gLevelScript.point;
        angle = AI_AngleToPoint(obj, point);
        if (ABS(angle) <= 0.1f) {
            obj->m_vVelocity.y += 0.0102400007f; /* 0x3C27C5AD: one ulp above 0.01024f */
            obj->m_vVelocity.x += func_002982F0(obj->m_RotY) * -24.576f;
            obj->m_vVelocity.z += func_002974C0(obj->m_RotY) * -24.576f;
            if (ai->m_AnimRepeatedCount != 0) {
                CAI__SetMode(obj, ai, 0x17);
                return;
            }
        }
        AI_TurnClamped(obj, ai, 0.261799425f, angle);
    }
    if (gLevelScript.bossMode == 9) {
        if (gLevelScript.swayTimer <= 0.0f) {
            obj->m_vVelocity.y += gFrameIncrement * 0.001f;
            obj->m_vVelocity.x += -(gLevelScript.swayAmount * 10.24f)
                                * func_002982F0(obj->m_RotY + gLevelScript.swayPhase * 3.14159274f) * gFrameIncrement;
            obj->m_vVelocity.z += -(gLevelScript.swayAmount * 10.24f)
                                * func_002974C0(obj->m_RotY + gLevelScript.swayPhase * 3.14159274f) * gFrameIncrement;
        }
        if (ai->m_AnimRepeatedCount != 0) {
            gLevelScript.bossMode = 2;
            CAI__SetMode(obj, ai, 0x17);
        }
    }
}


extern LevelScriptState gLevelScript; /* local: globals/boss.h is not included here (its gLevelScript_timer clashes with this file's view) */
extern s32 GameObject_IsOnGround(CGameObjectInstance *obj);
extern BossTimers gLevelScript_timer_v0040688C __asm__("gLevelScript_timer"); /* local type: the field's real type (or the member path) changes the ROM's code here */

/* Pushes the target away from the boss while the push timer has run out. */
void Primagen_UpdateMode20_2(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;
    CVector3 pos;
    CVector3 d;
    CGameObjectInstance *target;
    f32 heading;
    f32 push;

    gCollisionInfoFlyingEnemy.m_dwFlags |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 0, 1);
    gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x8000;
    if (t.m_Sight.m_pTarget != NULL) {
        AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
        gLevelScript.targetAngle = t.m_Sight.m_Angle;
    } else {
        AI_TurnClamped(obj, ai, 0.261799425f, gLevelScript.targetAngle);
    }
    target = t.m_Sight.m_pTarget;
    if (target != NULL && gLevelScript_timer_v0040688C.pushTimer <= 0.0f) {
        pos = target->header.m_vPos;
        CVector3__Subtract(&d, &obj->header.m_vPos, &pos);
        heading = CVector3__GetYaw(&d);
        if (GameObject_IsOnGround(target)) {
            push = 716.8f;
        } else {
            push = 179.2f;
        }
        target->m_vVelocity.x += push * func_002982F0(heading);
        target->m_vVelocity.z += push * func_002974C0(heading);
        if (gLevelScript_timer_v0040688C.timer > 0.0f) {
            gLevelScript_timer_v0040688C.timer -= gFrameIncrement;
        }
    }
    if (ai->m_AnimRepeatedCount != 0 && gLevelScript_timer_v0040688C.timer <= 0.0f && gLevelScript_timer_v0040688C.pushTimer <= 0.0f) {
        gLevelScript_timer_v0040688C.index = -1;
        obj->m_AI.m_AnimSpeed = 1.0f;
        CAI__SetMode(obj, ai, 7);
    }
}

/* Primagen_SetSpawnScript_2: start the second script for a stage. */
static __inline__ void StartScript2(s32 stage) {
    gLevelScript.script2.cursor = -1;
    gLevelScript.script2On = 1;
    gLevelScript.script2.current = -1;
    if (stage == 0) {
        gLevelScript.script2.steps = NULL;
    }
    if (stage == 1) {
        gLevelScript.script2.steps = sPrimagenSpawnScript1_2;
    }
    if (stage == 2) {
        gLevelScript.script2.steps = sPrimagenSpawnScript2_2;
    }
    if (stage == 3) {
        gLevelScript.script2.steps = sPrimagenSpawnScript3_2;
    }
    if (stage == 4) {
        gLevelScript.script2.steps = sPrimagenSpawnScript4_2;
    }
    if (stage == 5) {
        gLevelScript.script2.steps = sPrimagenSpawnScript5_2;
    }
}

void Primagen_UpdateMode24_2(CGameObjectInstance *obj, CAI *ai) {
    f32 dy;

    if (ai->m_AnimRepeatedCount == 0) {
        return;
    }
    dy = obj->header.m_vPos.y - gLevelScript.dest.y;
    if (!(ABS(dy) < 10.0f)) {
        return;
    }
    obj->m_vVelocity.x = 0.0f;
    obj->m_vVelocity.y = 0.0f;
    obj->m_vVelocity.z = 0.0f;
    gLevelScript.unk3CA = 0;
    gLevelScript.script.current = -1;
    gCollisionInfoEnemyB.m_dwFlags &= ~0x300;
    gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x300;
    if (gLevelScript.bossState == 3) {
        obj->header.m_vPos.x = gLevelScript.home.x;
        obj->header.m_vPos.y = gLevelScript.home.y;
        obj->header.m_vPos.z = gLevelScript.home.z;
        obj->m_RotY = 0.0f;
        gLevelScript.bossMode = 1;
        if (gLevelScript.stage == 1) {
            StartScript2(1);
        }
        if (gLevelScript.stage == 2) {
            StartScript2(2);
        }
        if (gLevelScript.stage == 3) {
            StartScript2(3);
        }
        CAI__SetMode(obj, ai, 5);
    } else if (!gLevelScript.partActive) {
        gLevelScript.bossMode = 2;
        CAI__SetMode(obj, ai, 7);
    } else {
        CAI__SetMode(obj, ai, 0x10);
    }
}

/* An effect's parameters, passed by value. */
typedef struct {
    /* 0x0 */ s16 upper;
    /* 0x2 */ s16 lower;
    /* 0x4 */ u8 b[4];
} __attribute__((aligned(4))) EffectParams;
extern void ImpactEvent_Send(CGameObjectInstance *src, CGameObjectInstance *dst, s32 effect, s32 id,
                          CVector3 pos, EffectParams params);
extern s32 gLevelScript_values_v00406C4C[] __asm__("gLevelScript_values"); /* local type: values[0] as one row; the real s32 [2][8] (or the member path) changes the ROM code here. [0]: parts left (mode 1), [2]: parts left (mode 2, the old symbol at 0x8012F990) */

void Primagen_HandleAnimEvent_2(s32 arg0, CGameObjectInstance *obj, s32 arg2, CVector3 pos, s32 event) {
    CAITargets t;
    EffectParams params;
    f32 size = 10.0f; /* set before the calls: the ROM multiplies by 256 at run time */

    gCollisionInfoFlyingEnemy.m_dwFlags |= 0x8000;
    AI_UpdateTargets(obj, &obj->m_AI, &t, 1, 1);
    gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x8000;
    if (t.m_Sight.m_pTarget == NULL) {
        return;
    }
    if (event == 2 && t.m_Sight.m_Dist <= 409.59998f && gLevelScript_values_v00406C4C[2] > 0) {
        params.upper = 0xA00;
        params.lower = 0x400;
        params.b[0] = 0;
        params.b[1] = 0;
        params.b[2] = 0;
        params.b[3] = 0;
        ImpactEvent_Send(obj, obj, 3, -1, t.m_Sight.m_pTarget->header.m_vPos, params);
    }
    if (event == 1 && t.m_Sight.m_Dist <= 716.8f && gLevelScript_values_v00406C4C[0] > 0) {
        params.upper = size * 256.0f;
        params.lower = 0x800;
        params.b[0] = 0;
        params.b[1] = 0;
        params.b[2] = 0;
        params.b[3] = 0;
        ImpactEvent_Send(obj, obj, 3, -1, t.m_Sight.m_pTarget->header.m_vPos, params);
    }
}

/* Pick the next attack of the script: steps are skipped by chance, type 5 wraps around. */
void Primagen_NextAttackStep_2(void) {
    if (gLevelScript.script.cursor != -1) {
        gLevelScript.script.timer = gLevelScript.script.steps[gLevelScript.script.cursor].delay;
    } else {
        gLevelScript.script.timer = 0.0f;
    }
    do {
        gLevelScript.script.cursor++;
        if (gLevelScript.script.steps[gLevelScript.script.cursor].type == 5) {
            gLevelScript.script.cursor = 0;
        }
        gLevelScript.script.current = gLevelScript.script.cursor;
    } while ((f32) gLevelScript.script.steps[gLevelScript.script.cursor].chance <= Random_Blend(0.0f, 100.0f));
}

void Primagen_SetAttackScript_2(s32 arg0) {
    if (arg0 == 2) {
        if (gLevelScript.stage == 1) {
            gLevelScript.script.steps = sPrimagenAttackScript2Stage1_2;
        }
        if (gLevelScript.stage == arg0) {
            gLevelScript.script.steps = sPrimagenAttackScript2Stage2_2;
        }
        if (gLevelScript.stage == 3) {
            gLevelScript.script.steps = sPrimagenAttackScript2Stage3_2;
        }
        gLevelScript.script.cursor = -1;
        gLevelScript.script.current = -1;
    }
    if (arg0 == 1) {
        if (gLevelScript.stage == arg0) {
            gLevelScript.script.steps = sPrimagenAttackScriptStage1_2;
        }
        if (gLevelScript.stage == 2) {
            gLevelScript.script.steps = sPrimagenAttackScriptStage2_2;
        }
        if (gLevelScript.stage == 3) {
            gLevelScript.script.steps = sPrimagenAttackScriptStage3_2;
        }
        gLevelScript.script.cursor = -1;
        gLevelScript.script.current = -1;
    }
}

extern void *CLevelActorPool__Spawn(CLevelActorPool *table, void *slot, s32 kind, CVector3 velocity, CVector3 pos,
                           void *region, f32 arg);
extern void Actor_SpawnParticle(void *actor, s32 event, CVector3 pos, s32 arg);

/* Spawn an object of the given kind at pos, thrown upwards, and send it event 0x11D. */
void Primagen_SpawnTossedObject_2(s32 kind, s32 count, CVector3 pos) {
    CVector3 velocity;
    void *region;
    void *spawned;

    region = CScene__FindRegionAt(&gScene, &pos);
    velocity.x = 0.0f;
    velocity.y = 512.0f;
    velocity.z = 0.0f;
    spawned = CLevelActorPool__Spawn(&gScene.m_ActorPool, NULL, kind, velocity, pos, region, count);
    if (spawned != NULL) {
        Actor_SpawnParticle(spawned, 0x11D, pos, 0);
    }
}

#define SCRIPT gLevelScript.script2.steps

void Primagen_SetSpawnScript_2(s32 arg0) {
    gLevelScript.script2.cursor = -1;
    gLevelScript.script2On = 1;
    gLevelScript.script2.current = -1;
    if (arg0 == 0) {
        SCRIPT = NULL;
    }
    if (arg0 == 1) {
        SCRIPT = sPrimagenSpawnScript1_2;
    }
    if (arg0 == 2) {
        SCRIPT = sPrimagenSpawnScript2_2;
    }
    if (arg0 == 3) {
        SCRIPT = sPrimagenSpawnScript3_2;
    }
    if (arg0 == 4) {
        SCRIPT = sPrimagenSpawnScript4_2;
    }
    if (arg0 == 5) {
        SCRIPT = sPrimagenSpawnScript5_2;
    }
}

/* Two words at gLevelScript_script3_cursor; the pointer just before them (gLevelScript_script3_steps) picks
 * the data for the given mode. */

void Primagen_SetSoundScript_2(s32 arg0) {
    gLevelScript.script3.cursor = -1;
    gLevelScript.script3.current = -1;
    if (arg0 == 0) {
        gLevelScript.script3.steps = NULL;
    }
    if (arg0 == 1) {
        gLevelScript.script3.steps = sPrimagenSoundScript1_2;
    }
    if (arg0 == 4) {
        gLevelScript.script3.steps = sPrimagenSoundScript4_2;
    }
    if (arg0 == 2) {
        gLevelScript.script3.steps = sPrimagenSoundScript2_2;
    }
    if (arg0 == 3) {
        gLevelScript.script3.steps = sPrimagenSoundScript3_2;
    }
}


extern void Primagen_Effects();

void Primagen_UpdateMode0_2(CGameObjectInstance *obj, CAI *ai) {
    Callbacks *found[10];
    Callbacks *item;
    s32 n;
    s32 i;

    n = CScene__FindInstances(&gScene, 0x453, 9, -1, found, 10);
    for (i = 0; i < n; i++) {
        item = found[i];
        item->onDraw = Primagen_Draw_2;
        item->onUpdate = Primagen_Effects;
    }
    CAI__SetMode(obj, ai, 0x1C);
}

void Primagen_EnterMode1_2(CGameObjectInstance *obj) {
    if (gLevelScript_bossState.mode == 5) {
        gLevelScript_bossState.state = 5;
    } else if (gLevelScript_bossState.mode == 6) {
        gLevelScript_bossState.state = 6;
    } else if (gLevelScript_bossState.mode == 10) {
        gLevelScript_bossState.state = 10;
    } else {
        gLevelScript_bossState.state = 6;
    }
    obj->m_dwFlags |= 0x2000;
}

s32 AI_StartSound(sptr, sptr, s32);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Sound__Play_Words Sound__Play_i3
s32 Sound__Play_i3(s32, s32, sptr, sptr, s32, s32);
#else
/* Sound__Play with its CVector3 as three words (an asm label, as code/CPlayer's) */
s32 Sound__Play_Words(s32, s32, sptr, sptr, s32, s32) __asm__("Sound__Play");
#endif
/* Field access by byte offset: a struct member reference here would let the
 * scheduler reorder these stores around the global ones. */
#define OBJ_FIELD(obj, type, field) (*(type *)((s8 *)(obj) + (s32) &((CGameObjectInstance *)0)->field))

extern s8 gLevelScript_unk3CA_v004072C8[] __asm__("gLevelScript_unk3CA"); /* local type: the field's real type (or the member path) changes the ROM's code here */

void Primagen_EnterMode2_2(void *arg0, sptr arg1) {
    OBJ_FIELD(arg0, f32, m_AI.m_AnimSpeed) = 1.0f;
    gLevelScript_unk3CA_v004072C8[0] = 0;
    *(s32 *)&gLevelScript_unk3CA_v004072C8[0x1A] = 0;
    OBJ_FIELD(arg0, s32, m_dwFlags) |= 0x2000;
    Sound__Play_Words(0x2779, *(s32 *)&((CGameObjectInstance *)arg0)->header.m_vPos.x, *(s32 *)&((CGameObjectInstance *)arg0)->header.m_vPos.y, *(s32 *)&((CGameObjectInstance *)arg0)->header.m_vPos.z, 0, -1);
    AI_StartSound((sptr) arg0, arg1, 0x3B21);
}

/* Two flags at gLevelScript_armed; the word 8 bytes before it is gLevelScript_bossState. */
extern BossFlags gLevelScript_armed_v0040734C __asm__("gLevelScript_armed"); /* local type: the field's real type (or the member path) changes the ROM's code here */

void Primagen_EnterMode5_2(CGameObjectInstance *arg0) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
    gLevelScript_armed_v0040734C.unk0 = 0;
    gLevelScript_armed_v0040734C.unk1 = 0;
    ((s32 *) &gLevelScript_armed_v0040734C)[-2] = 1;
}

void Primagen_UpdateMode5_2(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        gLevelScript_bossMode.mode = 5;
        gLevelScript_bossMode.timer = Random_Blend(0.5f, 4.0f) * 15.0f;
        CAI__SetMode(obj, ai, 1);
    }
    if (gLevelScript_bossMode.mode == 0) {
        CAI__SetMode(obj, ai, 6);
    }
}

void Primagen_EnterMode6_2(CGameObjectInstance *arg0) {
    s32 *flags;

    /* through a pointer: a plain struct store gets scheduled after gLevelScript_bossState's */
    flags = &arg0->m_dwFlags;
    *flags = (s32) (*flags | 0x2000);
    gLevelScript_bossState.state = 0;
}

/* host body (round 10 y2): gLevelScript by member. The view anchored at found[1] reached bossMode
 * at the N64 delta +0x64; on the host found[] holds 8-byte pointers and bossMode sits further on:
 * `mode = 8` hit another word, bossMode stayed 1 and the Primagen idled in mode 18 for good (its
 * update only acts in boss modes 3, 4, 8 and 9): no attack, no part ever opened. */
void Primagen_UpdateMode6_2(sptr arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(gLevelScript.found[1], &gLevelScript.found[1]->m_AI, 1);
        CAI__SetMode(gLevelScript.found[0], &gLevelScript.found[0]->m_AI, 1);
        gLevelScript.bossMode = 8;
        gCollisionInfoEnemyB.m_dwFlags |= 0x300;
        gCollisionInfoFlyingEnemy.m_dwFlags |= 0x300;
        CAI__SetMode(arg0, ai, 0x12);
    }
}

void Primagen_EnterMode19_2(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    gCollisionInfoFlyingEnemy.m_dwFlags |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 0, 1);
    gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x8000;
    obj->m_AI.m_AnimSpeed = 1.0f;
    obj->m_dwFlags |= 0x2000;
    if (gLevelScript.bossMode == 11) {
        gLevelScript.bossState = 11;
    }
    if (gLevelScript.bossMode == 4) {
        if (gLevelScript.stage != 1 && gLevelScript.bossState != 4) {
            gLevelScript_path = 0;
            sPrimagenPathPoint = 0;
            gLevelScript.dest = gPrimagenPaths[0][0];
        }
        gLevelScript.bossState = 4;
        gLevelScript.unk3CC = 3;
    }
    if (gLevelScript.bossMode == 3) {
        gLevelScript.bossState = 3;
        gLevelScript.unk3CC = 3;
    }
}

/* Host body: arg0 + 0x1FC is m_AI.m_AnimSpeed and + 0xD4 m_dwFlags at N64 offsets (as
 * Primagen_EnterMode19_2 has them typed); the gLevelScript_bossMode words (mode, timer at +8,
 * swayTimer at +0xC, unk3CA at -0xA) are a run of scalars, the same deltas on the host. */
void Primagen_EnterMode18_2(void *arg0)
{
  CGameObjectInstance *obj = (CGameObjectInstance *) arg0;

  obj->m_AI.m_AnimSpeed = 1.0f;
  if ((*((s32 *) (((s8 *) (&gLevelScript_bossMode)) + 0))) == 3)
  {
    *((f32 *) (((s8 *) (&gLevelScript_bossMode)) + 8)) = 18.75f;
  }
  if ((*((s32 *) (((s8 *) (&gLevelScript_bossMode)) + 0))) == 4)
  {
    *((s8 *) (((s8 *) (&gLevelScript_bossMode)) + (-0xA))) = 1;
    *((f32 *) (((s8 *) (&gLevelScript_bossMode)) + 8)) = 18.75f;
  }
  obj->m_dwFlags |= 0x2000;
  *((f32 *) (((s8 *) (&gLevelScript_bossMode)) + 0xC)) = 15.0f;
}

void Primagen_EnterMode20_2(CGameObjectInstance *obj) {
    obj->m_dwFlags |= 0x2000;
    gLevelScript_bossMode.swayTimer = 15.0f;
    gLevelScript_bossMode.timer = 11.25f;
}

void Primagen_EnterMode23_2(CGameObjectInstance *obj) {
    f32 speed = 1.0f;

    obj->m_AI.m_AnimSpeed = speed;
    gLevelScript.timer = speed;
    obj->m_dwFlags |= 0x2000;
}

void Primagen_UpdateMode23_2(CGameObjectInstance *obj, CAI *ai) {
    CVector3 flat;

    if (ai->m_AnimRepeatedCount == 0) {
        return;
    }
    if (!(obj->header.m_vPos.y < 10.0f)) {
        return;
    }
    gLevelScript.script.current = -1;
    gLevelScript.unk3CA = 0;
    gLevelScript.bossMode = 2;
    flat.x = obj->header.m_vPos.x;
    flat.y = 0.0f;
    flat.z = obj->header.m_vPos.z;
    if (func_00298470(flat.x * flat.x + flat.z * flat.z) < 1638.3999f) {
        gCollisionInfoEnemyB.m_dwFlags &= ~0x300;
        gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x300;
    }
    if (!gLevelScript.partActive) {
        CAI__SetMode(obj, ai, 7);
    } else {
        CAI__SetMode(obj, ai, 0x10);
    }
}

/* Fields addressed relative to gLevelScript_timer; the mode word sits 0xC before it. */

/* The port (round 12 a2): State_004077AC views LevelScriptState from its timer with N64 deltas (0x50 dest.y,
 * 0x5C home.y, -0xC bossState); the host's script blocks hold pointers, so dest and home sit later
 * and the write missed dest.y: mode 24 then waited for good for the Primagen to reach a dest.y it
 * never set (stage 2's bossState 6 path, he lands on the floor at y 0). Members instead. */
void Primagen_EnterMode24_2(CGameObjectInstance *actor) {
    actor->m_AI.m_AnimSpeed = 1.0f;
    actor->m_dwFlags |= 0x2000;
    gLevelScript.timer = 1.0f;
    if (gLevelScript.bossState == 3) {
        gLevelScript.dest.y = gLevelScript.home.y;
    } else {
        gLevelScript.dest.y = 0.0f;
    }
}

#include "m2c_macros.h"

void Primagen_EnterMode21_2(CGameObjectInstance *arg0) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
}

void Primagen_UpdateMode21_2(CGameObjectInstance *obj, CAI *ai) {
    f32 *animSpeed = &obj->m_AI.m_AnimSpeed;

    if (ai->m_AnimRepeatedCount != 0) {
        *animSpeed = 1.0f;
        gLevelScript.script.current = -1;
        CAI__SetMode(obj, ai, 0x13);
    }
}

void Primagen_EnterMode22_2(CGameObjectInstance *obj) {
    obj->m_vVelocity.x = 0.0f;
    obj->m_vVelocity.y = 0.0f;
    obj->m_vVelocity.z = 0.0f;
    obj->m_dwFlags = (s32) (obj->m_dwFlags | 0x2000);
}

void Primagen_UpdateMode22_2(CGameObjectInstance *obj, CAI *ai) {
    f32 *animSpeed;

    if (ai->m_AnimRepeatedCount != 0) {
        /* through a pointer: a plain struct store gets scheduled later */
        animSpeed = &obj->m_AI.m_AnimSpeed;
        *animSpeed = 1.0f;
        gLevelScript.script.current = -1;
        CAI__SetMode(obj, ai, 0x13);
    }
}

void Primagen_EnterMode27_2(CGameObjectInstance *arg0) {
    arg0->m_AI.m_AnimSpeed = 1.0f;
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
}

void Primagen_EnterMode9_2(void) {

}

void Primagen_UpdateMode9_2(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    gCollisionInfoFlyingEnemy.m_dwFlags |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x8000;
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount == 0) {
        return;
    }
    gLevelScript_bossState.script.current = -1;
    obj->m_AI.m_AnimSpeed = 1.0f;
    if (gLevelScript_bossState.state == 6 || t.m_Sight.m_Dist <= 460.8f) {
        gLevelScript_bossState.mode = 6;
        CAI__SetMode(obj, ai, 1);
    } else if (gLevelScript_bossState.state == 2) {
        CAI__SetMode(obj, ai, 7);
    }
}

void Primagen_EnterMode10_2(void) {

}

void Primagen_UpdateMode10_2(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    gCollisionInfoFlyingEnemy.m_dwFlags |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x8000;
    if (t.m_Sight.m_pTarget != NULL) {
        AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
        gLevelScript.targetAngle = t.m_Sight.m_Angle;
    } else {
        AI_TurnClamped(obj, ai, 0.261799425f, gLevelScript.targetAngle);
    }
    if (ai->m_AnimRepeatedCount != 0) {
        obj->m_AI.m_AnimSpeed = 1.0f;
        gLevelScript_bossState.script.current = -1;
        gLevelScript_bossState.mode = 6;
        CAI__SetMode(obj, ai, 1);
    }
}

void Primagen_EnterMode11_2(void) {

}

extern s32 gLevelScript_script_current_v00407AC8[] __asm__("gLevelScript_script_current"); /* local type: the field's real type (or the member path) changes the ROM's code here */        /* boss script index; an array so the store stays after the obj store */

void Primagen_UpdateMode11_2(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;

    gCollisionInfoFlyingEnemy.m_dwFlags |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    gCollisionInfoFlyingEnemy.m_dwFlags &= ~0x8000;
    AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount != 0) {
        obj->m_AI.m_AnimSpeed = 1.0f;
        gLevelScript_script_current_v00407AC8[0] = -1;
        CAI__SetMode(obj, ai, 7);
    }
}

void Primagen_UpdateMode14_2(CGameObjectInstance *obj, CAI *ai) {
    s32 var_a2;

    if (ai->m_AnimRepeatedCount != 0) {
        var_a2 = 0x13;
        if (gLevelScript.unk3CA == 0) {
            var_a2 = 7;
        }
        CAI__SetMode(obj, ai, var_a2);
    }
}

/* LevelScriptState (structs/boss.h) from scriptCOn on, gLevelScript_scriptCOn (+0x3C8):
 * scriptCOn and script3. */


void Primagen_EnterMode16_2(void) {
    gLevelScript.scriptCOn = 1;
    gLevelScript.script3.cursor = -1;
    gLevelScript.script3.current = -1;
    gLevelScript.script3.steps = sPrimagenSoundScript4_2;
}

void Primagen_UpdateMode16_2(sptr arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        gLevelScript.scriptCOn = 0;
        CAI__SetMode(arg0, ai, 2);
    }
}

void Primagen_EnterMode28_2(void) {

}

s32 COnScreen__SetBoss(sptr, sptr, sptr);

/* Host body: the ROM writes script3 and script2 at N64 deltas from script3.cursor (-4 the steps,
 * -0xC script2On, -0x14/-0x10 script2's cursor/current, -0x18 script2's steps): with the host's
 * 8-byte step pointers those deltas land in the wrong members (script3.steps half-written: the
 * first Primagen_NextSoundStep_2 crashed). The same stores by member. */
void Primagen_UpdateMode28_2(sptr arg0, sptr arg1)
{
  COnScreen__SetBoss((sptr) (&gEngineApp_m_OnScreen), arg0, (sptr) "Primagen");
  gLevelScript.script3.cursor = -1;
  gLevelScript.script3.steps = sPrimagenSoundScript3_2;
  gLevelScript.script2On = 1;
  gLevelScript.script3.current = -1;
  gLevelScript.script2.cursor = -1;
  gLevelScript.script2.current = -1;
  gLevelScript.script2.steps = sPrimagenSpawnScript1_2;
  CAI__SetMode(arg0, arg1, 5);
}

#include "structs/player.h"
#include "protos/player.h"
/* The object this callback gets (only +0xF8 is used). */
#ifdef TARGET_PORT
/* the CAI's m_pfHurt at its host offset (tools/port/hostview.py; round 10 y2): the N64 0xF8
 * cleared other bytes, the hurt callback stayed */
typedef struct {
    union {
        struct { u8 _hp_held[__builtin_offsetof(CAI, m_pfHurt)]; void *held; }; /* N64 0xF8 */
    };
} Holder;
#else
typedef struct {
    /* 0x00 */ u8 pad00[0xF8];
    /* 0xF8 */ void *held;
} Holder;
#endif

extern void CPlayerPool__SetInvincibility(CPlayerPool *list, f32 arg1);
extern void Cinema_RequestById_2(s32 id);

void Primagen_EnterDeath_2(sptr arg0, Holder *obj) {
    CPlayer *player;
    GameSettings *flags;

    obj->held = NULL;
    CPlayerPool__SetInvincibility(&gEngineApp_m_Game.m_PlayerList, 900.0f);
    player = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    player->m_Data.m_bPrimagenBossComplete = 1;
    if (player->m_Data.m_bLevel1TotemFlags && player->m_Data.m_bLevel2TotemFlags && player->m_Data.m_bLevel3TotemFlags
        && player->m_Data.m_bLevel4TotemFlags && player->m_Data.m_bLevel5TotemFlags) {
        Cinema_RequestById_2(0x172);
        (&gEngineApp_m_Game.m_Settings)->unlockedCheats |= 0x7FF0019;
    } else {
        Cinema_RequestById_2(0x173);
        flags = &gEngineApp_m_Settings;
        flags->unlockedCheats |= 2;
    }
}

s32 CPlayer__Warp(sptr, s32, s32, s32);
s32 Cinema_IsIdle();                                /* extern */

void Primagen_UpdateDeath_2(void) {
    CPlayer *temp_v0;

    if (Cinema_IsIdle() != 0) {
        temp_v0 = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
        if (temp_v0 != NULL) {
            temp_v0->m_Data.m_bPrimagenBossComplete = 1;
            CPlayer__Warp((sptr) temp_v0, 0x1F40, 2, 0);
        }
    }
}

#include "structs/light.h"
#include "structs/model.h"
#include "protos/game.h"

s32 Cinema_PathPlaying(void);
void CGameObjectInstance__DrawShadow(void *actor, void *state, CDrawArgs *req);
void ScriptAnimator_Apply(Slot00407DC0 *slot);
void ScreenTint_UpdateMode(void);

static __inline__ s32 hasBit(s32 mask, s32 bit) {
    return mask & bit;
}

/* Draws each of the boss's 8 parts present in its mask; part 1 shows mesh 8 in state 29. */
void Primagen_Draw_2(u8 *actor, u8 *state, CDrawArgs *req) {
    CGameObjectInstance *boss = gLevelScript.actor;
    s32 mask;
    s32 i;

    mask = boss->m_AI.m_VisBits;
    for (i = 0; i < 8; i++) {
        if (hasBit(mask, 1 << i)) {
            if (i == 1 && boss->m_AI.m_Mode == 29) {
                req->m_nModel = 8;
                req->m_nShadowModel = 8;
            } else {
                req->m_nModel = i;
                req->m_nShadowModel = i;
            }
            CGameObjectInstance__DrawShadow(actor, state, req);
            if (!Cinema_PathPlaying()) {
                ScriptAnimator_Apply(&gLevelScript.anims[i]);
                ScreenTint_UpdateMode();
            }
            /* the actor's draw matrices, lights and texture by member (N64 0x98, 0x110, 0x151) */
            Render_DrawModel(req->m_hModel, ((CGameObjectInstance *) actor)->m_pmtDrawMtxs, 1,
                             (char *) &((CGameObjectInstance *) actor)->m_Lights[gEvenOdd], 0, req->m_nModel,
                             ((CGameObjectInstance *) actor)->m_AI.m_nTexture);
            gBlendState = 0;
        }
    }
}

void func_00440F0C(void) {

}
