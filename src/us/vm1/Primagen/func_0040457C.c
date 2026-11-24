/* Each spawn sets nextWave in one place (`if (spawnsLeft == 0 && CurrentWave()->type != 1) {...} else nextWave = 1;`): written as an else-if with two `nextWave = 1`, the extra refs give nextWave $s2 and actor $s3, the ROM's two swapped. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/game.h"
#include "structs/boss.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "protos/ai.h"
#include "structs/vector.h"
#include "globals/collision.h"
#include "structs/twin_primagen.h"



#ifdef TARGET_PORT
typedef struct Actor0040457C {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Actor0040457C(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_ai[T2_HOFF_Actor0040457C(m_AI.m_dwFlags)]; u8 ai[1]; }; /* N64 0x140 */
    };
} Actor0040457C;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Actor0040457C;
#endif

/* gLevelScript is LevelScriptState (structs/boss.h); this file keeps its own
 * view because the wave spawner reads script2 as its own wave table (steps
 * laid out as Wave0040457C, not BossStep2: steps = waves, current = wave,
 * script2On = wavesDone) and found[] as its own actor type. */
#ifdef TARGET_PORT
typedef struct ModeState0040457C {
/* the view at the host offsets of LevelScriptState (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ModeState0040457C(m) __builtin_offsetof(LevelScriptState, m)
    union {
        struct { u8 _hp_mode[T2_HOFF_ModeState0040457C(stage)]; s32 mode; }; /* N64 0x0 */
        struct { u8 _hp_handle[T2_HOFF_ModeState0040457C(intel)]; sptr handle; }; /* host: the intelligence pointer */ /* N64 0x344 */
        struct { u8 _hp_spawnsLeft[T2_HOFF_ModeState0040457C(spawnsLeft)]; s32 spawnsLeft; }; /* N64 0x348 */
        struct { u8 _hp_wavesLeft[T2_HOFF_ModeState0040457C(wavesLeft)]; s32 wavesLeft; }; /* N64 0x34C */
        struct { u8 _hp_enemiesAlive[T2_HOFF_ModeState0040457C(enemiesAlive)]; s32 enemiesAlive; }; /* N64 0x350 */
        struct { u8 _hp_spawnTimer[T2_HOFF_ModeState0040457C(spawnTimer)]; f32 spawnTimer; }; /* N64 0x354 */
        struct { u8 _hp_region[T2_HOFF_ModeState0040457C(region)]; sptr region; }; /* host: a region pointer */ /* N64 0x358 */
        struct { u8 _hp_doorsOpen[T2_HOFF_ModeState0040457C(doorsOpen)]; u8 doorsOpen; }; /* N64 0x35C */
        struct { u8 _hp_waveTimer[T2_HOFF_ModeState0040457C(waveTimer)]; f32 waveTimer; }; /* N64 0x360 */
        struct { u8 _hp_nextTimer[T2_HOFF_ModeState0040457C(nextTimer)]; f32 nextTimer; }; /* N64 0x364 */
        struct { u8 _hp_actor[T2_HOFF_ModeState0040457C(actor)]; void *actor; }; /* N64 0x368 */
        struct { u8 _hp_found[T2_HOFF_ModeState0040457C(found)]; Actor0040457C *found[5]; }; /* N64 0x36C */
        struct { u8 _hp_unk3D0[T2_HOFF_ModeState0040457C(bossState)]; s32 unk3D0; }; /* N64 0x3D0 */
        struct { u8 _hp_unk3D4[T2_HOFF_ModeState0040457C(bossMode)]; s32 unk3D4; }; /* N64 0x3D4 */
        struct { u8 _hp_unk400[T2_HOFF_ModeState0040457C(script2.timer)]; f32 unk400; }; /* N64 0x400 */
        struct { u8 _hp_waves[T2_HOFF_ModeState0040457C(script2.steps)]; Wave0040457C *waves; }; /* N64 0x404 */
        struct { u8 _hp_wave[T2_HOFF_ModeState0040457C(script2.current)]; s32 wave; }; /* N64 0x40C */
        struct { u8 _hp_wavesDone[T2_HOFF_ModeState0040457C(script2On)]; u8 wavesDone; }; /* N64 0x410 */
        struct { u8 _hp_unk420[T2_HOFF_ModeState0040457C(script3.current)]; s32 unk420; }; /* N64 0x420 */
        struct { u8 _hp_resetRegion[T2_HOFF_ModeState0040457C(resetRegion)]; s32 resetRegion; }; /* N64 0x424 */
    };
} ModeState0040457C;
#else
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
#endif


#ifdef TARGET_PORT
typedef struct Info0040457C {
/* the view at the host offsets of CAITargets (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Info0040457C(m) __builtin_offsetof(CAITargets, m)
    union {
        struct { u8 _hp_unk24[T2_HOFF_Info0040457C(m_Sight.m_Dist)]; s32 unk24; }; /* N64 0x24 */
        u8 _hp_whole[sizeof(CAITargets)]; /* the whole CAITargets */
    };
} Info0040457C;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad1[0x88 - 0x28];
} Info0040457C;
#endif

extern ModeState0040457C gLevelScript_v0040457C __asm__("gLevelScript"); /* local type: globals/boss.h declares it; this view (its type here) is what keeps the ROM's code */
CGameObjectInstance *CPlayerPool__FindNearest(void *game, CVector3 pos);
f32 Random_Blend(f32 lo, f32 hi);
struct CGameObjectInstance *CInstancePool__Spawn(void *, s32, s32, void *, void *, CVector3, f32, CVector3, CVector3, s32 *);
void AI_UpdateTargets(CGameObjectInstance *, CAI *, CAITargets *, s32, s32);
void Primagen_NextSpawnStep(void);
s32 Primagen_RunSpawnStep(void);
/* the ROM's call passes the actor, the CAI and the target's word in a0-a2 (the definition reads none) */
#define Primagen_RunSpawnStep_3 ((void (*)(CGameObjectInstance *, void *, s32)) Primagen_RunSpawnStep)
void Primagen_NextSoundStep(void);
s32 Primagen_RunSoundStep(CGameObjectInstance *);
#define Primagen_RunSoundStep_3 ((void (*)(CGameObjectInstance *, void *, s32)) Primagen_RunSoundStep)

static __inline__ Wave0040457C *CurrentWave(void) {
    return (Wave0040457C *)(gLevelScript_v0040457C.wave * sizeof(Wave0040457C) + (sptr)gLevelScript_v0040457C.waves);
}

void Primagen_UpdateSpawner(CGameObjectInstance *actor, void *level) {
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
        Primagen_NextSpawnStep();
    }
    Primagen_RunSpawnStep_3(actor, level, info.unk24);
    if (gLevelScript_v0040457C.unk420 == -1) {
        Primagen_NextSoundStep();
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
