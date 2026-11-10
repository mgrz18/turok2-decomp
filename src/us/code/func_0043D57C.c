/* Second VM module twin of Primagen_UpdateSpawner (same source, same link address). */
/* Each spawn sets nextWave in one place (`if (spawnsLeft == 0 && CurrentWave()->type != 1) {...} else nextWave = 1;`): written as an else-if with two `nextWave = 1`, the extra refs give nextWave $s2 and actor $s3, the ROM's two swapped. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "protos/ai.h"

typedef struct {
    f32 x, y, z;
} Vec3_0040457C;

typedef struct {
    /* 0x00 */ u8 flag;
    /* 0x01 */ u8 pad01[0x8 - 0x1];
    /* 0x08 */ s32 count;
    /* 0x0C */ f32 delay;
    /* 0x10 */ u8 pad10[0x14 - 0x10];
    /* 0x14 */ s32 type;
    /* 0x18 */ u8 pad18[0x1C - 0x18];
} Wave0040457C;

typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Actor0040457C;

/* gLevelScript is LevelScriptState (structs/boss.h); this file keeps its own
 * view because the wave spawner reads script2 as its own wave table (steps
 * laid out as Wave0040457C, not BossStep2: steps = waves, current = wave,
 * script2On = wavesDone) and found[] as its own actor type. */
typedef struct {
    /* 0x000 */ s32 mode;
    /* 0x004 */ u8 pad004[0x344 - 0x4];
    /* 0x344 */ s32 handle;
    /* 0x348 */ s32 spawnsLeft;
    /* 0x34C */ s32 wavesLeft;
    /* 0x350 */ s32 enemiesAlive;
    /* 0x354 */ f32 spawnTimer;
    /* 0x358 */ s32 region;
    /* 0x35C */ u8 doorsOpen;
    /* 0x35D */ u8 pad35D[0x360 - 0x35D];
    /* 0x360 */ f32 waveTimer;
    /* 0x364 */ f32 nextTimer;
    /* 0x368 */ void *actor;
    /* 0x36C */ Actor0040457C *found[5];
    /* 0x380 */ u8 pad380[0x3D0 - 0x380];
    /* 0x3D0 */ s32 unk3D0;
    /* 0x3D4 */ s32 unk3D4;
    /* 0x3D8 */ u8 pad3D8[0x400 - 0x3D8];
    /* 0x400 */ f32 unk400;
    /* 0x404 */ Wave0040457C *waves;
    /* 0x408 */ u8 pad408[0x40C - 0x408];
    /* 0x40C */ s32 wave;
    /* 0x410 */ u8 wavesDone;
    /* 0x411 */ u8 pad411[0x420 - 0x411];
    /* 0x420 */ s32 unk420;
    /* 0x424 */ s32 resetRegion;
} ModeState0040457C;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_0040457C pos;
    /* 0x10 */ s32 region;
    /* 0x14 */ u8 pad14[0x18 - 0x14];
    /* 0x18 */ Vec3_0040457C rot;
} Entity0040457C;

typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad1[0x88 - 0x28];
} Info0040457C;

extern ModeState0040457C gLevelScript; /* local type: globals/boss.h declares it; this view (its type here) is what keeps the ROM's code */
extern s32 D_800F1F50;
Entity0040457C *CPlayerPool__FindNearest(void *game, Vec3_0040457C pos);
f32 Random_Blend(f32 lo, f32 hi);
s32 CInstancePool__Spawn(void *a0, s32 a1, s32 model, sptr handle, sptr region, Vec3_0040457C pos, f32 a6, Vec3_0040457C rot, Vec3_0040457C scale, sptr a9);
void AI_UpdateTargets(Entity0040457C *actor, void *level, Info0040457C *info, s32 a3, s32 a4);
void Primagen_NextSpawnStep_2(void);
void Primagen_RunSpawnStep_2(Entity0040457C *actor, void *level, s32 a2);
void Primagen_NextSoundStep_2(void);
void Primagen_RunSoundStep_2(Entity0040457C *actor, void *level, s32 a2);

static __inline__ Wave0040457C *CurrentWave(void) {
    return (Wave0040457C *)(gLevelScript.wave * sizeof(Wave0040457C) + (sptr)gLevelScript.waves);
}

void Primagen_UpdateSpawner_2(Entity0040457C *actor, void *level) {
    Info0040457C info;
    Vec3_0040457C scale;
    Vec3_0040457C pos;
    s32 nextWave = 0;
    Entity0040457C *player;
    CListedInstance *enemy;
    Wave0040457C *wave;
    s32 extra;
    f32 delay;
    s32 *flags;

    player = CPlayerPool__FindNearest(&gEngineApp_m_Game.m_PlayerList, actor->pos);
    if (player == NULL) {
        return;
    }
    if (gLevelScript.resetRegion != 0) {
        gLevelScript.region = player->region;
        gLevelScript.resetRegion = 0;
    }
    if (!gLevelScript.doorsOpen) {
        if (gLevelScript.spawnsLeft > 0) {
            gLevelScript.doorsOpen = 1;
            CAI__SetMode(gLevelScript.found[2], gLevelScript.found[2]->ai, 0);
            CAI__SetMode(gLevelScript.found[3], gLevelScript.found[3]->ai, 0);
        }
    }
    if (gLevelScript.doorsOpen) {
        if (gLevelScript.spawnsLeft == 0 && gLevelScript.spawnTimer <= 0.0f) {
            gLevelScript.doorsOpen = 0;
            CAI__SetMode(gLevelScript.found[2], gLevelScript.found[2]->ai, 1);
            CAI__SetMode(gLevelScript.found[3], gLevelScript.found[3]->ai, 1);
        }
    }
    if (gLevelScript.wavesLeft > 0 && gLevelScript.nextTimer <= 0.0f && gLevelScript.wave != -1) {
        delay = CurrentWave()->delay;
        extra = CurrentWave()->count;
        gLevelScript.wavesLeft--;
        if (gLevelScript.wavesLeft == 0) {
            gLevelScript.wavesDone = 1;
            gLevelScript.wave = -1;
        } else if (extra != 3) {
            extra -= 3;
            gLevelScript.nextTimer = delay - extra * 15 + Random_Blend(0.0f, extra * 2) * 15.0f;
        } else {
            gLevelScript.nextTimer = delay;
        }
        CAI__SetMode(gLevelScript.found[4], gLevelScript.found[4]->ai, 0);
    }
    if ((u32)(gLevelScript.spawnsLeft - 1) < 8 && gLevelScript.waveTimer <= 0.0f) {
        pos.x = 1669.12f;
        pos.y = 1064.96f;
        pos.z = 2816.0f;
        scale.x = 2.0f;
        scale.y = 2.0f;
        scale.z = 2.0f;
        if (CInstancePool__Spawn(&gScene.m_InstancePool, -1, 0x387, gLevelScript.handle, gLevelScript.region, pos, 0.0f, actor->rot,
                          scale, 0)) {
            gLevelScript.spawnsLeft--;
            if (gLevelScript.waves != NULL) {
                if (gLevelScript.spawnsLeft == 0 && CurrentWave()->type != 1) {
                    gLevelScript.wavesDone = 1;
                    gLevelScript.wave = -1;
                } else {
                    nextWave = 1;
                }
            }
            gLevelScript.spawnTimer = 120.0f;
        }
    }
    if ((u32)(gLevelScript.spawnsLeft - 1) < 8 && gLevelScript.waveTimer <= 0.0f) {
        pos.x = -1699.84f;
        pos.y = 1064.96f;
        pos.z = 2877.44f;
        scale.x = 2.0f;
        scale.y = 2.0f;
        scale.z = 2.0f;
        if (CInstancePool__Spawn(&gScene.m_InstancePool, -1, 0x387, gLevelScript.handle, gLevelScript.region, pos, 0.0f, actor->rot,
                          scale, 0)) {
            gLevelScript.spawnsLeft--;
            if (gLevelScript.waves != NULL) {
                if (gLevelScript.spawnsLeft == 0 && CurrentWave()->type != 1) {
                    gLevelScript.wavesDone = 1;
                    gLevelScript.wave = -1;
                } else {
                    nextWave = 1;
                }
            }
        }
    }
    if (nextWave && gLevelScript.wave != -1) {
        delay = CurrentWave()->delay;
        extra = CurrentWave()->count;
        if (gLevelScript.waves != NULL) {
            if (extra != 3) {
                extra -= 3;
                gLevelScript.waveTimer = delay - extra * 15 + Random_Blend(0.0f, extra * 2) * 15.0f;
            } else {
                gLevelScript.waveTimer = delay;
            }
        }
    }
    gLevelScript.enemiesAlive = 0;
    for (enemy = gScene.m_InstancePool.m_ActiveList.m_pHead; enemy != NULL; enemy = enemy->m_pNext) {
        enemy->m_Instance.m_AI.m_vLeashPos.x = 0;
        enemy->m_Instance.m_AI.m_vLeashPos.y = 768.0f;
        enemy->m_Instance.m_AI.m_vLeashPos.z = 0;
        enemy->m_Instance.m_AI.m_pLeashRegion = (struct CGameRegion *)gLevelScript.region;
        if (enemy->m_Instance.m_AI.m_Health > 0) {
            gLevelScript.enemiesAlive++;
        }
    }
    flags = &D_800F1F50;
    *flags |= 0x8000;
    AI_UpdateTargets(actor, level, &info, 0, 1);
    *flags &= ~0x8000;
    if (gLevelScript.wave == -1) {
        Primagen_NextSpawnStep_2();
    }
    Primagen_RunSpawnStep_2(actor, level, info.unk24);
    if (gLevelScript.unk420 == -1) {
        Primagen_NextSoundStep_2();
    }
    Primagen_RunSoundStep_2(actor, level, info.unk24);
    if (gLevelScript.waves != NULL) {
        if (gLevelScript.enemiesAlive == 0) {
            wave = CurrentWave();
            if (wave->type == 1 && wave->flag == 1 && gLevelScript.spawnsLeft == 0 && gLevelScript.unk400 <= 0.0f) {
                gLevelScript.wavesDone = 1;
                gLevelScript.wave = -1;
            }
        }
        if (gLevelScript.waves != NULL) {
            return;
        }
    }
    if (gLevelScript.unk3D0 == 1) {
        gLevelScript.unk3D4 = 0;
    }
}
