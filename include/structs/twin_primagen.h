/* Local types that vm1/Primagen and its vm2 copy both define (one layout): one definition
 * for both copies (tools/struct_census.py). */
#ifndef STRUCTS_TWIN_PRIMAGEN_H
#define STRUCTS_TWIN_PRIMAGEN_H

#include "common.h"
#include "structs/game_object.h"
#include "structs/game.h"
#include "structs/boss.h"
#include "structs/vector.h"
#include "structs/light.h"
#include "structs/model.h"

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
    /* 0x0 */ f32 waitMin;       /* 0x8012F618 */
    /* 0x4 */ f32 waitMax;       /* 0x8012F61C */
} BossWait;
typedef struct {
    u8 data[0x64];
} Slot00407DC0;

/* LevelScriptState from spawnsLeft on (gLevelScript_spawnsLeft, +0x348): Primagen_NextSpawnStep and
 * Primagen_RunSpawnStep reach script2 through that symbol. */
#ifdef TARGET_PORT
typedef struct LevelScriptF950 {
/* the view at the host offsets of LevelScriptState (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_LevelScriptF950(m) __builtin_offsetof(LevelScriptState, m)
    union {
        struct { u8 _hp_spawnsLeft[T2_HOFF_LevelScriptF950(spawnsLeft) - T2_HOFF_LevelScriptF950(spawnsLeft)]; s32 spawnsLeft; }; /* N64 0x0 */
        struct { u8 _hp_wavesLeft[T2_HOFF_LevelScriptF950(wavesLeft) - T2_HOFF_LevelScriptF950(spawnsLeft)]; s32 wavesLeft; }; /* N64 0x4 */
        struct { u8 _hp_enemiesAlive[T2_HOFF_LevelScriptF950(enemiesAlive) - T2_HOFF_LevelScriptF950(spawnsLeft)]; s32 enemiesAlive; }; /* N64 0x8 */
        struct { u8 _hp_script2[T2_HOFF_LevelScriptF950(script2) - T2_HOFF_LevelScriptF950(spawnsLeft)]; BossScript2 script2; }; /* N64 0xB8 */
        struct { u8 _hp_script2On[T2_HOFF_LevelScriptF950(script2On) - T2_HOFF_LevelScriptF950(spawnsLeft)]; u8 script2On; }; /* N64 0xC8 */
    };
} LevelScriptF950;
#else
typedef struct {
    /* 0x00 */ s32 spawnsLeft;    /* 0x8012F950 */
    /* 0x04 */ s32 wavesLeft;
    /* 0x08 */ s32 enemiesAlive;
    /* 0x0C */ u8 pad0C[0xB8 - 0xC];
    /* 0xB8 */ BossScript2 script2; /* 0x8012FA08 */
    /* 0xC8 */ u8 script2On;      /* 0x8012FA18 */
} LevelScriptF950;
#endif
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
} ModePair;
typedef struct {
    /* 0x0 */ s8 unk0;
    /* 0x1 */ s8 unk1;
} BossFlags;
typedef struct {
    f32 unk_0;                  /* 0x00 */
    char pad4[0x50 - 0x4];
    f32 unk_50;                 /* 0x50 */
    char pad54[0x5C - 0x54];
    f32 unk_5C;                 /* 0x5C */
} State_004077AC;
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
#ifdef TARGET_PORT
typedef struct BossCtl {
/* the view at the host offsets of LevelScriptState (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_BossCtl(m) __builtin_offsetof(LevelScriptState, m)
    union {
        struct { u8 _hp_unk0[T2_HOFF_BossCtl(unk3CC) - T2_HOFF_BossCtl(unk3CC)]; s32 unk0; }; /* N64 0x0 */
        struct { u8 _hp_state[T2_HOFF_BossCtl(bossState) - T2_HOFF_BossCtl(unk3CC)]; s32 state; }; /* N64 0x4 */
        struct { u8 _hp_dest[T2_HOFF_BossCtl(dest) - T2_HOFF_BossCtl(unk3CC)]; CVector3 dest; }; /* N64 0x5C */
    };
} BossCtl;
#else
typedef struct {
    /* 0x00 */ s32 unk0;   /* 0x8012F9D4 */
    /* 0x04 */ s32 state;  /* 0x8012F9D8 */
    /* 0x08 */ u8 pad8[0x5C - 0x8];
    /* 0x5C */ CVector3 dest; /* 0x8012FA30 */
} BossCtl;
#endif
/* Boss module state from 0x8012F9E4. */
#ifdef TARGET_PORT
typedef struct BossTimers {
/* the view at the host offsets of LevelScriptState (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_BossTimers(m) __builtin_offsetof(LevelScriptState, m)
    union {
        struct { u8 _hp_timer[T2_HOFF_BossTimers(timer) - T2_HOFF_BossTimers(timer)]; f32 timer; }; /* N64 0x0 */
        struct { u8 _hp_pushTimer[T2_HOFF_BossTimers(swayTimer) - T2_HOFF_BossTimers(timer)]; f32 pushTimer; }; /* N64 0x4 */
        struct { u8 _hp_index[T2_HOFF_BossTimers(script.current) - T2_HOFF_BossTimers(timer)]; s32 index; }; /* N64 0x20 */
    };
} BossTimers;
#else
typedef struct {
    /* 0x00 */ f32 timer;           /* 0x8012F9E4 */
    /* 0x04 */ f32 pushTimer;       /* 0x8012F9E8 */
    /* 0x08 */ u8 pad08[0x18];
    /* 0x20 */ s32 index;           /* 0x8012FA04 */
} BossTimers;
#endif
/* gLevelScript_script2_cursor: a small state block; the word before it (gLevelScript_script2_steps) is the
 * script pointer chosen here. */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s8 unk8;
} State_004070B4;
/* Just the AI callbacks this sets. */
#ifdef TARGET_PORT
/* the CAI's draw and effects callbacks at their host offsets (tools/port/hostview.py; round 10
 * y2): the N64 0x234/0x248 wrote the two pointers into other members of the 0x453 actors (the
 * Primagen's parts never got their draw/pose callbacks) */
#define T2_HOFF_Callbacks(m) __builtin_offsetof(CGameObjectInstance, m)
typedef struct {
    union {
        struct { u8 _hp_onDraw[T2_HOFF_Callbacks(m_AI.m_pfDraw)]; void *onDraw; }; /* N64 0x234 */
        struct { u8 _hp_onUpdate[T2_HOFF_Callbacks(m_AI.m_pfEffects)]; void *onUpdate; }; /* N64 0x248 */
    };
} Callbacks;
#else
typedef struct {
    /* 0x000 */ u8 pad000[0x234];
    /* 0x234 */ void *onDraw;
    /* 0x238 */ u8 pad238[0x10];
    /* 0x248 */ void *onUpdate;
} Callbacks;
#endif

#endif
