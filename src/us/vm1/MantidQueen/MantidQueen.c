#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/scene.h"
#include "globals/boss.h"
#include "protos/scene.h"
#include "protos/ai.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/engine.h"
#include "globals/onscreen.h"
#include "m2c_macros.h"
/* the boss by member (round 9 x2): gMantidQueen.boss is a pointer, the ROM's s32 symbol cut it to its
 * low half (--high-memory) and its N64 AI offset (+ 0x140) is not the host CAI's */
#include "structs/boss.h"
#include "protos/player.h"
#include "structs/player.h"
#include "structs/light.h"
#include "structs/model.h"
#include "protos/game.h"
#include "structs/vector.h"
#include "globals/boss_ai.h"

/* MantidQueen_NextStep */
static __inline__ void State_NextStep(WaveBossState *s) {
    WaveBossStep *step;
    s32 value;

    s->step++;
    s->stepIndex++;
    step = s->step;
    value = step->spawnCount;
    s->timer = 0;
    s->toSpawn += value;
    if (step->partPicks != 0 && step->spawnCount != 0) {
        s->waveActive = 1;
    } else {
        s->waveActive = 0;
    }
    if (s->stepIndex == 7) {
        CAI__SetMode(s->actor, ((u8 *)&s->actor->m_AI), 14);
    } else if (s->stepIndex == 8) {
        CAI__SetMode(s->actor, ((u8 *)&s->actor->m_AI), 16);
    }
}

/* MantidQueen_CountActiveMinions */
static __inline__ s32 State_CountActive(WaveBossState *s) {
    s32 i;
    s32 n = 0;
    CListedInstance *e;

    for (i = 0; i < s->memberCount; i++) {
        if (s->members[i]->m_AI.m_Mode == 1) {
            n++;
        }
    }
    for (e = gScene.m_InstancePool.m_ActiveList.m_pHead; e != NULL; e = e->m_pNext) {
        if (e->m_Instance.m_AI.m_Health > 0) {
            n++;
        }
    }
    return n;
}

/* MantidQueen_UpdateBossAnim */
static __inline__ void State_UpdateBoss(WaveBossState *s) {
    CGameObjectInstance *boss = s->boss;
    s32 aiState;

    boss->m_dwFlags &= ~0x10000000;
    aiState = boss->m_AI.m_VarBuffer[0];
    if (aiState == 0x5AB4) {
    } else if (aiState < 0x5AB5) {
    } else if (aiState == 0x5ABE) {
        if (boss->m_AI.m_AnimRepeatedCount != 0) {
            AI_PlayAnim(s->boss, ((u8 *)&s->boss->m_AI), 0x5ADC);
        }
    }
}

void ScriptAnimator_Start(void *slot, void *actor);

static __inline__ void State_SetStep(WaveBossState *s, WaveBossStep *step, s32 index) {
    s->step = step;
    s->stepIndex = index;
    s->toSpawn += step->spawnCount;
    s->timer = 0;
    if (step->partPicks != 0 && step->spawnCount != 0) {
        s->waveActive = 1;
    } else {
        s->waveActive = 0;
    }
}

void MantidQueen_Init(WaveBossState *s, CGameObjectInstance *actor) {
    s32 i;

    s->actor = actor;
    for (i = 0; i < 5; i++) {
        s->spots[i].health = 0x1900;
        ScriptAnimator_Start(&s->spots[i].anim, actor);
    }
    s->spots[1].health = 0xFF00;
    s->spots[2].health = 0x9600;
    s->spots[3].health = 0x1E00;
    s->spots[4].health = 0x1900;
    s->toSpawn = 0;
    s->spots[0].health = 0;
    State_SetStep(s, sMantidQueenScript, 1);
    CScene__FindInstances(&gScene, 0x457, 9, 0x1D4D, &s->boss, 1);
    s->memberCount = CScene__FindInstances(&gScene, 0x651, 10, 0x1D4D, s->members, 8);
    for (i = 0; i < 2; i++) {
        s->intel[i] = CScene__FindIntelligence(&gScene, 1, sMantidQueenMinions[i].id);
    }
    s->blink = 0;
    s->blinkTimer = 0.0f;
}

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ CVector3 pos;
} Entity00401FCC;
typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[0x174 - 0x140];
    /* 0x174 */ union {
        s32 word;
        s8 state;
    } u174;
    /* 0x178 */ u8 pad1[0x1FC - 0x178];
    /* 0x1FC */ f32 delay;
    /* 0x200 */ u8 pad2[0x24C - 0x200];
    /* 0x24C */ f32 distance;
    /* 0x250 */ s32 unk250;
} Member00401FCC;
Entity00401FCC *CPlayerPool__FindNearest(void *game, CVector3 pos);
f32 AI_DistSqToPointAtHeight(Member00401FCC *member, CVector3 pos);
f32 Random_Blend(f32 lo, f32 hi);
void Sort_Quick(void *base, s32 count, s32 size, s32 (*cmp)(), void (*swap)());
typedef struct {
    f32 x, y, z;
} Vec3_00402BB0;
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* the views at the host offsets (round 9 x2): the instances' position and region pointer
 * (header.m_vPos, header.m_pCurrentRegion: the region was kept in an s32) and the CAI's
 * m_vDesiredPos (N64 0x54) */
typedef struct {
    union {
        struct { u8 _hp_pos[__builtin_offsetof(CGameObjectInstance, header.m_vPos)]; Vec3_00402BB0 pos; };
    };
} Marker00402BB0;

typedef struct {
    union {
        struct { u8 _hp_pos[__builtin_offsetof(CGameObjectInstance, header.m_vPos)]; Vec3_00402BB0 pos; };
        struct { u8 _hp_region[__builtin_offsetof(CGameObjectInstance, header.m_pCurrentRegion)]; sptr region; };
    };
} Obj00402BB0;

typedef struct {
    union {
        struct { u8 _hp_pos[__builtin_offsetof(CAI, m_vDesiredPos)]; Vec3_00402BB0 pos; };
    };
} Item00402BB0;
#else
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00402BB0 pos;
} Marker00402BB0;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00402BB0 pos;
    /* 0x10 */ s32 region;
} Obj00402BB0;

typedef struct {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ Vec3_00402BB0 pos;
} Item00402BB0;
#endif
void MantidQueen_UpdateIntro(Obj00402BB0 *obj, Item00402BB0 *item);
void MantidQueen_EnterMode2(CGameObjectInstance *arg0);
void MantidQueen_UpdateMode2(sptr arg0, sptr arg1);
void MantidQueen_EnterMode3(CGameObjectInstance *arg0);
void MantidQueen_UpdateMode3(CGameObjectInstance *obj, CAI *ai);
void MantidQueen_EnterMode4(CGameObjectInstance *arg0);
void MantidQueen_UpdateMode4(sptr arg0, CAI *arg1);
void MantidQueen_EnterMode5(CGameObjectInstance *arg0);
void MantidQueen_UpdateMode5(sptr arg0, sptr arg1);
void MantidQueen_UpdateMode6(sptr arg0, void *arg1);
void MantidQueen_EnterMode6(CGameObjectInstance *arg0);
void MantidQueen_EnterPush(void);
typedef struct {
    f32 x, y, z;
} Vec3_00402FA0;
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00402FA0 pos;
} Target00402FA0;
#ifdef TARGET_PORT
typedef struct {
    union {
        struct { u8 _hp_target[__builtin_offsetof(CAI, m_pSightTarget)]; Target00402FA0 *target; }; /* N64 0x80 */
        struct { u8 _hp_count[__builtin_offsetof(CAI, m_AnimRepeatedCount)]; s8 count; }; /* N64 0xC7 */
    };
} Item00402FA0;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ Target00402FA0 *target;
    /* 0x84 */ u8 pad1[0xC7 - 0x84];
    /* 0xC7 */ s8 count;
} Item00402FA0;
#endif
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* the views at the host offsets (tools/port/hostview.py; round 10 y2): the actor's position,
 * velocity and current animation (frame, index), the CAI's sight target and
 * m_AnimRepeatedCount. The N64 views read other bytes on the host: the push never ended (the
 * repeat count) and never pushed. */
#define T2_HOFF_Obj00402FA0(m) __builtin_offsetof(CGameObjectInstance, m)
typedef struct {
    union {
        struct { u8 _hp_pos[T2_HOFF_Obj00402FA0(header.m_vPos)]; Vec3_00402FA0 pos; }; /* N64 0x4 */
        struct { u8 _hp_vel[T2_HOFF_Obj00402FA0(m_vVelocity)]; Vec3_00402FA0 vel; }; /* N64 0x18 */
        struct { u8 _hp_time[T2_HOFF_Obj00402FA0(m_CurrentAnimation.m_AnimationFrame)]; f32 time; }; /* N64 0xD8 */
        struct { u8 _hp_anim[T2_HOFF_Obj00402FA0(m_CurrentAnimation.m_nAnimIndex)]; s16 anim; }; /* N64 0xDC */
    };
} Obj00402FA0;
#else
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00402FA0 pos;
    /* 0x10 */ u8 pad0[0x18 - 0x10];
    /* 0x18 */ Vec3_00402FA0 vel;
    /* 0x24 */ u8 pad1[0xD8 - 0x24];
    /* 0xD8 */ f32 time;
    /* 0xDC */ s16 anim;
} Obj00402FA0;
#endif
void MantidQueen_UpdatePush(Obj00402FA0 *obj, Item00402FA0 *item);
s16 MantidQueen_PickFromStep(void);
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* the view at the host offsets (round 9 x2, the Mantid Queen fight): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
typedef struct {
    union {
        struct { u8 _hp_flags[__builtin_offsetof(CAI, m_ModeFlags)]; s32 flags; };
        struct { u8 _hp_id[__builtin_offsetof(CAI, m_AnimType)]; s16 id; };
        struct { u8 _hp_count[__builtin_offsetof(CAI, m_AnimRepeatedCount)]; s8 count; };
    };
} Item004030D4;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x3C];
    /* 0x3C */ s32 flags;
    /* 0x40 */ u8 pad1[0xC4 - 0x40];
    /* 0xC4 */ s16 id;
    /* 0xC6 */ u8 pad2;
    /* 0xC7 */ s8 count;
} Item004030D4;
#endif
void MantidQueen_UpdateAttackAnim(sptr a0, Item004030D4 *item);
void MantidQueen_UpdateTurn(sptr a0, CAI *item);
void MantidQueen_UpdateMode13(sptr arg0, CAI *arg1);
void MantidQueen_EnterMode14(void);
void MantidQueen_UpdateMode14(sptr arg0, CAI *arg1);
void MantidQueen_EnterMode15(void);
void MantidQueen_UpdateMode15(sptr a0, sptr a1);
void MantidQueen_EnterDeath(void *arg0);
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* the views at the host offsets (tools/port/hostview.py; round 10 y2): the actor's m_dwFlags and
 * the CAI's m_pfHurt (N64 0xF8), cleared as a whole pointer (an s32 store left its high half) */
#define T2_HOFF_Obj0040336C(m) __builtin_offsetof(CGameObjectInstance, m)
#define T2_HOFF_Item0040336C(m) __builtin_offsetof(CAI, m)
typedef struct {
    union {
        struct { u8 _hp_flags[T2_HOFF_Obj0040336C(m_dwFlags)]; s32 flags; }; /* N64 0xD4 */
    };
} Obj0040336C;

typedef struct {
    union {
        struct { u8 _hp_unkF8[T2_HOFF_Item0040336C(m_pfHurt)]; void *unkF8; }; /* N64 0xF8 */
    };
} Item0040336C;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0xD4];
    /* 0xD4 */ s32 flags;
} Obj0040336C;

typedef struct {
    /* 0x00 */ u8 pad0[0xF8];
    /* 0xF8 */ s32 unkF8;
} Item0040336C;
#endif
void MantidQueen_UpdateDeath(Obj0040336C *obj, Item0040336C *item);
/* The controller of a boss fight (Owner00403430 in MantidQueen_UpdateBossAnim): it holds the boss at 0x220. */
#ifdef TARGET_PORT
#include "structs/boss.h"
/* the view at the host offsets (round 9 x2, the Mantid Queen fight): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
typedef struct {
    union {
        struct { u8 _hp_boss[__builtin_offsetof(WaveBossState, boss)]; CGameObjectInstance * boss; };
    };
} BossOwner;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x220];
    /* 0x220 */ CGameObjectInstance *boss;
} BossOwner;
#endif
void MantidQueen_SetBossAnim(BossOwner *arg0, s32 arg1);
#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/boss.h"
/* the view at the host offsets (round 9 x2, the Mantid Queen fight): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
typedef struct {
    union {
        struct { u8 _hp_flags[__builtin_offsetof(CGameObjectInstance, m_dwFlags)]; s32 flags; };
        struct { u8 _hp_ai[__builtin_offsetof(CGameObjectInstance, m_AI)]; u8 ai[sizeof(CAI)]; };
        struct { u8 _hp_active[__builtin_offsetof(CGameObjectInstance, m_AI.m_AnimRepeatedCount)]; s8 active; };
        struct { u8 _hp_state[__builtin_offsetof(CGameObjectInstance, m_AI.m_VarBuffer[0])]; s32 state; };
    };
} Actor00403430;
typedef struct {
    union {
        struct { u8 _hp_actor[__builtin_offsetof(WaveBossState, boss)]; Actor00403430 * actor; };
    };
} Owner00403430;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 pad1[0x140 - 0xD8];
    /* 0x140 */ u8 ai[0x207 - 0x140];
    /* 0x207 */ s8 active;
    /* 0x208 */ u8 pad2[0x24C - 0x208];
    /* 0x24C */ s32 state;
} Actor00403430;
typedef struct {
    /* 0x000 */ u8 pad0[0x220];
    /* 0x220 */ Actor00403430 *actor;
} Owner00403430;
#endif
void MantidQueen_UpdateBossAnim(Owner00403430 *owner);
void MantidQueen_FlashWeakSpot(void *state, void *arg);
typedef struct {
    /* 0x00 */ s32 ptr;
    /* 0x04 */ s32 size;
} Chunk;
typedef struct {
    /* 0x000 */ char pad0[0x214];
    /* 0x214 */ Chunk *chunk;
    /* 0x218 */ s32 total;
    /* 0x21C */ s32 pos;
    /* 0x220 */ char pad220[0x30];
    /* 0x250 */ s32 valid;
} Reader;
void MantidQueen_SetStep(Reader *r, Chunk *c);
void MantidQueen_NextStep(void *s_);
void MantidQueen_SortSwap(void **arg0, void **arg1);
s32 MantidQueen_SortCompare(CGameObjectInstance **arg0, CGameObjectInstance **arg1);

/* Host body (round 9 x2): the minions by member. The N64 view (Member00401FCC) put the CAI at
 * 0x140, the mode word at 0x174 (tested big-endian: mode 0, last mode 1), the anim speed at 0x1FC
 * and the sort key and flag at 0x24C/0x250 (m_AI.m_VarF32[0], m_VarS32[1]); and Sort_Quick
 * sorted the member pointers as 4-byte words. No minion was ever sent in: the wave's toSpawn
 * stayed 3 and the Queen's spots stayed shut. */
void MantidQueen_SendMinions(WaveBossState *s, s32 *active) {
    WaveBossStep *cfg = gMantidQueen.step;
    struct CPlayer *player;
    CVector3 pos;
    CGameObjectInstance *member;
    s32 i;

    player = (struct CPlayer *) CPlayerPool__FindNearest(&gEngineApp_m_Game.m_PlayerList, s->actor->header.m_vPos);
    if (player != NULL) {
        pos = player->m_CharacterActor.header.m_vPos;
    } else {
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
    }
    for (i = 0; i < s->memberCount; i++) {
        member = s->members[i];
        member->m_AI.m_VarF32[0] = AI_DistSqToPointAtHeight((Member00401FCC *) member, pos);
        member->m_AI.m_VarS32[1] = 1;
        if (member->m_AI.m_Mode == 0 && member->m_AI.m_ModeLastFrame == 1) {
            member->m_AI.m_AnimSpeed = Random_Blend(0.75f, 1.25f);
        }
    }
    Sort_Quick(s->members, s->memberCount, sizeof(s->members[0]), MantidQueen_SortCompare, MantidQueen_SortSwap);
    for (i = 0; i < s->memberCount && s->toSpawn != 0 && *active < cfg->maxActive; i++) {
        member = s->members[i];
        if (member->m_AI.m_Mode == 0 && CAI__SetMode(member, &member->m_AI, 1)) {
            member->m_AI.m_AnimSpeed = Random_Blend(1.5f, 2.0f);
            s->toSpawn--;
            (*active)++;
        }
    }
}

/* MantidQueen_NextStep */
static __inline__ void State_NextStep_4021C8(WaveBossState *s) {
    WaveBossStep *step;
    s32 value;

    s->step++;
    do {
        s->stepIndex++;
        step = s->step;
        value = step->spawnCount;
        s->timer = 0;
        s->toSpawn += value;
    } while (0);
    if (step->partPicks != 0 && step->spawnCount != 0) {
        s->waveActive = 1;
    } else {
        s->waveActive = 0;
    }
    if (s->stepIndex == 7) {
        CAI__SetMode(s->actor, ((u8 *)&s->actor->m_AI), 14);
    } else if (s->stepIndex == 8) {
        CAI__SetMode(s->actor, ((u8 *)&s->actor->m_AI), 16);
    }
}

void Animator_Update(void *slot);
s32 Cinema_PathPlaying(void);

void MantidQueen_Update(WaveBossState *s) {
    CGameObjectInstance *actor = s->actor;
    s32 active;
    s32 i;

    for (i = 0; i < 5; i++) {
        Animator_Update(&s->spots[i].anim);
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    if (actor->m_AI.m_Mode < 2) {
        return;
    }
    active = State_CountActive(s);
    if (s->toSpawn != 0) {
        MantidQueen_SendMinions(s, &active);
    }
    if (s->waveActive != 0 && s->toSpawn + active <= 0) {
        State_NextStep_4021C8(s);
    }
    State_UpdateBoss(s);
    s->blinkTimer -= gFrameIncrement;
    if (s->blinkTimer <= 0.0f) {
        s->blink ^= 1;
        if (s->blink) {
            s->blinkTimer = s->step->onTime;
        } else {
            s->blinkTimer = s->step->offTime;
        }
    }
}

/* MantidQueen_NextStep */
static __inline__ void State_NextStep_402404(WaveBossState *s) {
    WaveBossStep *step;
    s32 value;

    s->step++;
    do {
        s->stepIndex++;
        step = s->step;
        value = step->spawnCount;
        s->timer = 0;
        s->toSpawn += value;
    } while (0);
    if (step->partPicks != 0 && step->spawnCount != 0) {
        s->waveActive = 1;
    } else {
        s->waveActive = 0;
    }
    if (s->stepIndex == 7) {
        CAI__SetMode(s->actor, ((u8 *)&s->actor->m_AI), 14);
    } else if (s->stepIndex == 8) {
        CAI__SetMode(s->actor, ((u8 *)&s->actor->m_AI), 16);
    }
}

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

typedef struct {
    /* 0x00 */ u8 pad0[0x6C];
    /* 0x6C */ s32 mask;
} Source00402404;
#ifdef TARGET_PORT
#include "structs/collision.h"
/* the views at the host offsets of CAI (m_VisBits, m_Mode) and DamageEvent (damage, flags, frame)
 * (round 9 x2): the N64 0x34 read a byte of the host CAI's mode table pointer as the mode, the
 * hit's frame (the node mask) was read from its damage/radius words: no spot ever took a hit */
#define T2_HOFF_AI(m) __builtin_offsetof(CAI, m)
#define T2_HOFF_HIT(m) __builtin_offsetof(DamageEvent, m)
typedef struct {
    union {
        struct { u8 _hp_flags[T2_HOFF_AI(m_VisBits)]; s32 flags; };
        struct { u8 _hp_kind[T2_HOFF_AI(m_Mode)]; s8 kind; };
    };
} Obj00402404;

typedef struct {
    union {
        struct { u8 _hp_damage[T2_HOFF_HIT(damage)]; s32 damage; };
        struct { u8 _hp_flags[T2_HOFF_HIT(flags)]; s32 flags; };
        struct { u8 _hp_source[T2_HOFF_HIT(frame)]; Source00402404 *source; };
    };
} Hit00402404;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 flags;
    /* 0x10 */ u8 pad1[0x34 - 0x10];
    /* 0x34 */ s8 kind;
} Obj00402404;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 damage;
    /* 0x08 */ u8 pad1[0xC - 0x8];
    /* 0x0C */ s32 flags;
    /* 0x10 */ Source00402404 *source;
} Hit00402404;
#endif

void ScriptAnimator_SetScript(void *slot, void *arg);
void COnScreen__SetBossPercent(void *a0, s32 percent);

void MantidQueen_Hurt(void *a0, Obj00402404 *obj, Hit00402404 *hit) {
    WaveBossState *s;
    WaveBossStep *step;
    s32 index;
    s32 bit;
    s32 sum;
    s32 i;
    s32 percent;

#ifdef TARGET_PORT
    {
        extern void T2Port_LogBossHurt(const char *who, void *obj, void *hit); /* DEBUG-ONLY */
        T2Port_LogBossHurt("queen", a0, hit);
    }
#endif
    if (hit->source == NULL) {
        return;
    }
    s = &gMantidQueen;
    step = s->step;
    if (s->waveActive != 0) {
        return;
    }
    index = step->weakSpot;
    if (index == -1) {
        return;
    }
    if (index == 3 && obj->kind != 15) {
        return;
    }
    bit = 1 << index;
    if (!(hit->source->mask & bit)) {
        return;
    }
    if (obj->kind != 15 && (hit->flags & 0xC00000)) {
        CAI__SetMode(a0, obj, 13);
    }
    s->spots[index].health -= hit->damage;
    ScriptAnimator_SetScript(&s->spots[index].anim, sMantidQueenSpotHitScript);
    if (s->spots[index].health <= 0) {
        s->spots[index].health = 0;
        obj->flags &= ~bit;
        if (obj->kind != 15) {
            CAI__SetMode(a0, obj, 13);
        }
        State_NextStep_402404(s);
    }
    sum = 0;
    for (i = 0; i < 5; i++) {
        sum += s->spots[i].health;
    }
    percent = (sum >> 8) * 100 / 460;
    percent = MIN(MAX(percent, 0), 100);
    COnScreen__SetBossPercent(&gEngineApp_m_OnScreen, percent);
}

s32 Cinema_IsIdle(void);
void COnScreen__SetBoss(void *a0, Obj00402BB0 *obj, void *a2);

DEFERRED void MantidQueen_UpdateIntro(Obj00402BB0 *obj, Item00402BB0 *item) {
    Marker00402BB0 *marker;

    if (Cinema_IsIdle()) {
        if (CScene__FindInstances(&gScene, 0x451, 9, 0x1D4D, &marker, 1)) {
            item->pos = marker->pos;
            obj->pos = marker->pos;
            obj->region = CScene__FindRegionAt(&gScene, &obj->pos);
        }
        CAI__SetMode(obj, item, 8);
        COnScreen__SetBoss(&gEngineApp_m_OnScreen, obj, "Queen");
    }
}

DEFERRED void MantidQueen_EnterMode2(CGameObjectInstance *arg0) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x2000);
    CScene__MarkGeneratorsSaved(&gScene);
}

DEFERRED void MantidQueen_UpdateMode2(sptr arg0, sptr arg1) {
    if (gMantidQueen.waveActive == 0) {
        CAI__SetMode(arg0, arg1, 3);
    }
}

#define T2_QUEEN_PLAY(anim) AI_PlayAnim(gMantidQueen.boss, &gMantidQueen.boss->m_AI, (anim))

DEFERRED void MantidQueen_EnterMode3(CGameObjectInstance *arg0) {
    T2_QUEEN_PLAY(0x5AAA);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
}

DEFERRED void MantidQueen_UpdateMode3(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(obj, ai, 7);
    }
}

DEFERRED void MantidQueen_EnterMode4(CGameObjectInstance *arg0) {
    T2_QUEEN_PLAY(0x5AB4);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & 0xFFFEFFFF);
}

f32 Math_DecayToZero(f32, f32);

DEFERRED void MantidQueen_UpdateMode4(sptr arg0, CAI *arg1) {
    f32 temp_f20;

    temp_f20 = 51.199997f;
    arg1->m_vDesiredPos.x = Math_DecayToZero(arg1->m_vDesiredPos.x, temp_f20);
    arg1->m_vDesiredPos.z = Math_DecayToZero(arg1->m_vDesiredPos.z, temp_f20);
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 5);
    }
}

DEFERRED void MantidQueen_EnterMode5(CGameObjectInstance *arg0) {
    s32 *flags = &arg0->m_dwFlags;

    *flags = (s32) (*flags & ~0x2000);
    T2_QUEEN_PLAY(0x5ABE);    /* obj, &obj->m_AI */
}

DEFERRED void MantidQueen_UpdateMode5(sptr arg0, sptr arg1) {
    if (gMantidQueen.waveActive == 0) {
        CAI__SetMode(arg0, arg1, 6);
    }
}

#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define AI_AngleToPoint AI_AngleToPoint_i3
#endif

f32 AI_AngleToPoint(s32, s32, s32, s32);
void ScriptAnimator_SetScript(void *, void *);
#ifndef TARGET_PORT
/* gMantidQueen as bytes: through this view the ROM's code (an asm label, the header's name keeps its type) */
extern u8 gMantidQueen_bytes[] __asm__("gMantidQueen");
#endif
#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/player.h"
#undef AI_AngleToPoint
extern f32 AI_AngleToPoint(void *actor, CVector3 point);

/* Host body: the ROM reads the actor's yaw (0x50) and flags (0xD4), the CAI's
 * m_AnimRepeatedCount (0xC7), the player's position (4..0xC) and gMantidQueen's boss (0x220),
 * step (0x214) and weak spots (0x8 + 0x68 * i, the animator at +4) at N64 offsets. */
DEFERRED void MantidQueen_UpdateMode6(sptr arg0, void *arg1)
{
  CGameObjectInstance *obj = (CGameObjectInstance *) arg0;
  CAI *ai = (CAI *) arg1;
  WaveBossState *s = &gMantidQueen;
  CGameObjectInstance *player;
  s32 spot;

  player = (CGameObjectInstance *) CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
  if (player != NULL)
  {
    obj->m_RotY += AI_AngleToPoint(obj, player->header.m_vPos);
  }
  obj->m_dwFlags |= 0x2000;
  if (ai->m_AnimRepeatedCount != 0)
  {
    AI_PlayAnim((sptr) s->boss, &s->boss->m_AI, 0x5ADC);
    CAI__SetMode(obj, ai, 7);
    spot = s->step->weakSpot;
    if (spot != -1)
    {
      ScriptAnimator_SetScript(&s->spots[spot].anim, sMantidQueenMode6SpotScript);
    }
  }
}
#else
DEFERRED void MantidQueen_UpdateMode6(sptr arg0, void *arg1)
{
  s32 *var_s1;
  sptr var_s0;
  void *temp_v0;
  void *var_s2;
  s32 temp_v1;
  int new_var;
  var_s0 = arg0;
  var_s2 = arg1;
  temp_v0 = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
  var_s1 = &gMantidQueen_bytes;
  var_s1 = &gMantidQueen_bytes;
  if (temp_v0 != 0)
  {
    new_var = 0xC;
    *((f32 *) (((s8 *) var_s0) + 0x50)) = (f32) ((*((f32 *) (((s8 *) var_s0) + 0x50))) + AI_AngleToPoint(var_s0, *((s32 *) (((s8 *) temp_v0) + 4)), *((s32 *) (((s8 *) temp_v0) + 8)), *((s32 *) (((s8 *) temp_v0) + new_var))));
  }
  *((s32 *) (((s8 *) var_s0) + 0xD4)) = (s32) ((*((s32 *) (((s8 *) var_s0) + 0xD4))) | 0x2000);
  if ((*((s8 *) (((s8 *) var_s2) + 0xC7))) != 0)
  {
    AI_PlayAnim(*((s32 *) (((s8 *) (&gMantidQueen_bytes)) + 0x220)), (*((s32 *) (((s8 *) (&gMantidQueen_bytes)) + 0x220))) + 0x140, 0x5ADC);
    CAI__SetMode(var_s0, var_s2, 7);
    temp_v1 = *((s32 *) (((s8 *) (*((void **) (((s8 *) (&gMantidQueen_bytes)) + 0x214)))) + 0x14));
    if (temp_v1 != (-1))
    {
      ScriptAnimator_SetScript((temp_v1 * 0x68) + ((u8 *) (gMantidQueen_bytes + new_var)), sMantidQueenMode6SpotScript);
    }
  }
}
#endif

DEFERRED void MantidQueen_EnterMode6(CGameObjectInstance *arg0) {
    T2_QUEEN_PLAY(0x5AB4);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x10000);
}

DEFERRED void MantidQueen_EnterPush(void) {
    gMantidQueen.pushed = 0;
}

sptr CGameObjectInstance__PickRandomAnim(Obj00402FA0 *obj, sptr a1, sptr a2);
void CVector3__Subtract(Vec3_00402FA0 *out, Vec3_00402FA0 *from, Vec3_00402FA0 *to);

DEFERRED void MantidQueen_UpdatePush(Obj00402FA0 *obj, Item00402FA0 *item) {
    Vec3_00402FA0 dir;
    sptr anim;
    WaveBossState *s = &gMantidQueen;

    anim = CGameObjectInstance__PickRandomAnim(obj, 0x1004, -1);
    if (obj->anim == anim) {
        if (s->pushed == 0 && obj->time >= 10.0f) {
            s->pushed = 1;
            if (item->target != NULL) {
                CVector3__Subtract(&dir, &item->target->pos, &obj->pos);
                obj->vel.x += dir.x * 0.75f;
                obj->vel.z += dir.z * 0.75f;
            }
        }
        if (obj->time >= 20.0f) {
            obj->vel.x = 0.0f;
            obj->vel.z = 0.0f;
        }
    }
    if (item->count != 0) {
        CAI__SetMode(obj, item, 8);
    }
}

s16 WeightedTable_PickStatic(void *); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */

DEFERRED s16 MantidQueen_PickFromStep(void) {
    return WeightedTable_PickStatic(gMantidQueen.step->picks10);
}

DEFERRED void MantidQueen_UpdateAttackAnim(sptr a0, Item004030D4 *item) {
    s32 needed;

    switch (item->id) {
        case 0xFA2:
        case 0x10CC:
            item->flags &= ~2;
            break;
    }
    if (item->id == 0x10CC || item->id == 0x1131) {
        needed = 2;
    } else {
        needed = 1;
    }
    if (item->count >= needed) {
        CAI__SetMode(a0, item, 8);
    }
}

void AI_UpdateTargets(void *, void *, CAITargets *, s32, s32);
s32 AI_IsAimedAt(void *, void *, sptr, f32);

static __inline__ s32 func_00403774_inline(sptr a0, CAI *item, sptr a2) {
    s32 result;

    if (gMantidQueen.blink == 0) {
        result = 0;
    } else {
        result = AI_IsAimedAt((void *) a0, item, a2, 0.785398245f);
    }
    return result;
}

DEFERRED void MantidQueen_UpdateTurn(sptr a0, CAI *item) {
    CAITargets info;
    s32 side;

    AI_UpdateTargets((void *) a0, item, &info, 1, 1);
    AI_TurnClamped((void *) a0, (sptr) item, 0.17453295f, info.m_Path.m_Angle);
    side = func_00403774_inline(a0, item, (sptr) info.m_Sight.m_pTarget);
    if (side == -1) {
        CAI__SetMode((void *) a0, item, 11);
    } else if (side == 1) {
        CAI__SetMode((void *) a0, item, 12);
    } else if (item->m_AnimRepeatedCount >= 4) {
        CAI__SetMode((void *) a0, item, 8);
    }
}

DEFERRED void MantidQueen_UpdateMode13(sptr arg0, CAI *arg1) { /* the actor pointer-wide (sptr: s32 on the N64) */
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 8);
    }
}

DEFERRED void MantidQueen_EnterMode14(void) {
    CScene__UnmarkGeneratorsSaved(&gScene);
}

DEFERRED void MantidQueen_UpdateMode14(sptr arg0, CAI *arg1) { /* the actor pointer-wide (sptr: s32 on the N64) */
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 0xF);
    }
}

DEFERRED void MantidQueen_EnterMode15(void) {
    WaveBossState *s = &gMantidQueen;
    s32 index = s->step->weakSpot;

    if (index != -1) {
        ScriptAnimator_SetScript(&s->spots[index].anim, sMantidQueenSpotOpenScript);
    }
}

DEFERRED void MantidQueen_UpdateMode15(sptr a0, sptr a1) {
    CAITargets buf;

    AI_UpdateTargets((void *) a0, (void *) a1, &buf, 1, 1);
    AI_TurnClamped((void *) a0, a1, 0.0174532961f, buf.m_Path.m_Angle);
}

/* Turok 2 differs from Rage Wars here: no trigger fields are read; it calls
 * CPlayerPool__SetInvincibility on the player list with 900.0f, then Cinema_RequestById(0x136). */
void CPlayerPool__SetInvincibility(CPlayerPool *, f32);
s32 Cinema_RequestById(s32);

DEFERRED void MantidQueen_EnterDeath(void *arg0) {
    CPlayerPool__SetInvincibility(&gEngineApp_m_Game.m_PlayerList, 900.0f);
    Cinema_RequestById(0x136);
}

void CPlayerPool__SetInvincibility(CPlayerPool *list, f32 value);
void CPlayer__Warp(CPlayer *player, s32 a1, s32 a2, s32 a3);

DEFERRED void MantidQueen_UpdateDeath(Obj0040336C *obj, Item0040336C *item) {
    CGame *game = &gEngineApp_m_Game;
    CPlayer *player;

    CPlayerPool__SetInvincibility(&game->m_PlayerList, 900.0f);
    item->unkF8 = 0;
    obj->flags &= ~0x2000;
    if (Cinema_IsIdle()) {
        player = CPlayerPool__GetFirstActive(&game->m_PlayerList);
        if (player != NULL) {
            GameSettings *sub = &game->m_Settings;

            player->m_Data.m_bQueenBossComplete = 1;
            sub->unlockedCheats |= 0x6000;
            CPlayer__Warp(player, 0x176F, 2, 0);
        }
    }
}

DEFERRED void MantidQueen_SetBossAnim(BossOwner *arg0, s32 arg1) {
    CGameObjectInstance *temp_a0;

    temp_a0 = arg0->boss;
    AI_PlayAnim((sptr) temp_a0, &temp_a0->m_AI, arg1);
}

DEFERRED void MantidQueen_UpdateBossAnim(Owner00403430 *owner) {
    Actor00403430 *actor = owner->actor;
    s32 state;

    actor->flags &= ~0x10000000;
    state = actor->state;
    if (state == 0x5AB4) {
    } else if (state < 0x5AB5) {
    } else if (state == 0x5ABE) {
        if (actor->active != 0) {
            AI_PlayAnim(owner->actor, owner->actor->ai, 0x5ADC);
        }
    }
}

typedef struct {
    /* 0x00 */ s32 value;
    /* 0x04 */ u8 body[0x68 - 0x4];
} Slot00403494;
typedef struct {
    /* 0x000 */ u8 pad0[0x8];
    /* 0x008 */ Slot00403494 slots[5];
    /* 0x210 */ u8 pad1[0x214 - 0x210];
    /* 0x214 */ s32 *info;
} State00403494;

/* Host body: the step's weak spot (step + 0x14) and the spot's animator (0x8 + 0x68 * i + 4)
 * at N64 offsets; BossWeakSpot holds pointers on the host. */
DEFERRED void MantidQueen_FlashWeakSpot(void *state, void *arg) {
    WaveBossState *s = (WaveBossState *) state;
    s32 index = s->step->weakSpot;

    if (index != -1) {
        ScriptAnimator_SetScript(&s->spots[index].anim, arg);
    }
}

DEFERRED void MantidQueen_SetStep(Reader *r, Chunk *c) {
    r->chunk = c;
    r->total += c->size;
    r->pos = 0;
    if (c->ptr != 0 && c->size != 0) {
        r->valid = 1;
    } else {
        r->valid = 0;
    }
}

typedef struct {
    /* 0x00 */ s32 active;
    /* 0x04 */ s32 value;
    /* 0x08 */ u8 pad0[0x20 - 0x8];
} Step00403520;
typedef struct {
    /* 0x000 */ u8 *actor;
    /* 0x004 */ u8 pad0[0x210 - 0x4];
    /* 0x210 */ s32 stepIndex;
    /* 0x214 */ Step00403520 *step;
    /* 0x218 */ s32 total;
    /* 0x21C */ s32 timer;
    /* 0x220 */ u8 pad1[0x250 - 0x220];
    /* 0x250 */ s32 enabled;
} State00403520;
#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/boss.h"

/* Host body (round 9 x2): the state and the step by member (WaveBossState, WaveBossStep: the
 * N64 views' step, stepIndex, total (toSpawn), timer and enabled (waveActive), and the step's
 * active word (partPicks, a pointer), sit elsewhere once they hold 8-byte pointers), the actor's
 * CAI by member. */
DEFERRED void MantidQueen_NextStep(void *s_) {
    WaveBossState *s = s_;
    WaveBossStep *step;
    s32 value;

    s->step++;
    s->stepIndex++;
    step = s->step;
    value = step->spawnCount;
    s->timer = 0;
    s->toSpawn += value;
    if (step->partPicks != NULL && step->spawnCount != 0) {
        s->waveActive = 1;
    } else {
        s->waveActive = 0;
    }
    if (s->stepIndex == 7) {
        CAI__SetMode(s->actor, &s->actor->m_AI, 14);
    } else if (s->stepIndex == 8) {
        CAI__SetMode(s->actor, &s->actor->m_AI, 16);
    }
}
#else
/* Moves the encounter to its next step: accumulates the step's value, sets
 * whether it is enabled, and fires the actor's events for steps 7 and 8. */
DEFERRED void MantidQueen_NextStep(void *s_) {
    State00403520 *s = s_;
    Step00403520 *step;
    s32 value;

    s->step++;
    do {
        s->stepIndex++;
        step = s->step;
        value = step->value;
        s->timer = 0;
        s->total += value;
    } while (0);
    if (step->active != 0 && step->value != 0) {
        s->enabled = 1;
    } else {
        s->enabled = 0;
    }
    if (s->stepIndex == 7) {
        CAI__SetMode(s->actor, s->actor + 0x140, 14);
    } else if (s->stepIndex == 8) {
        CAI__SetMode(s->actor, s->actor + 0x140, 16);
    }
}
#endif

/* the members are host pointers (Sort_Quick is given their width) */
DEFERRED void MantidQueen_SortSwap(void **arg0, void **arg1) {
    void *t = *arg0;

    *arg0 = *arg1;
    *arg1 = t;
}

/* qsort comparator over actor pointers, by the float the AI keeps in its first variable
 * (m_AI.m_VarF32[0], N64 0x24C; round 9 x2: the host member) */
DEFERRED s32 MantidQueen_SortCompare(CGameObjectInstance **arg0, CGameObjectInstance **arg1) {
    s32 var_v0;

    var_v0 = 1;
    if ((*arg0)->m_AI.m_VarF32[0] < (*arg1)->m_AI.m_VarF32[0]) {
        var_v0 = -1;
    }
    return var_v0;
}

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/boss.h"
/* the view at the host offsets (round 9 x2, the Mantid Queen fight): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
typedef struct {
    union {
        struct { u8 _hp_alive[__builtin_offsetof(CGameObjectInstance, m_AI.m_Mode)]; s8 alive; };
    };
} Member004035F8;
typedef struct {
    union {
        struct { u8 _hp_members[__builtin_offsetof(WaveBossState, members)]; Member004035F8 * members[8]; };
        struct { u8 _hp_count[__builtin_offsetof(WaveBossState, memberCount)]; s32 count; };
    };
} Group004035F8;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x174];
    /* 0x174 */ s8 alive;
} Member004035F8;
typedef struct {
    /* 0x000 */ u8 pad0[0x224];
    /* 0x224 */ Member004035F8 *members[8];
    /* 0x244 */ s32 count;
} Group004035F8;
#endif

DEFERRED s32 MantidQueen_CountActiveMinions(Group004035F8 *group) {
    s32 i;
    s32 n = 0;
    CListedInstance *a;

    for (i = 0; i < group->count; i++) {
        if (group->members[i]->alive == 1) {
            n++;
        }
    }
    for (a = gScene.m_InstancePool.m_ActiveList.m_pHead; a != NULL; a = a->m_pNext) {
        if (a->m_Instance.m_AI.m_Health > 0) {
            n++;
        }
    }
    return n;
}

/* A weak spot seen from its animator: walking BossWeakSpot pointers and
 * taking &slot->anim moves the +4 into the loop and breaks the match. */
typedef struct {
    u8 data[sizeof(BossWeakSpot)]; /* the host stride of the weak spots (pointers in ScriptAnimator) */
} Slot00403660;
void CGameObjectInstance__DrawShadow(void *actor, void *state, CDrawArgs *req);
void ScriptAnimator_Apply(Slot00403660 *slot);
void ScreenTint_UpdateMode(void);

static __inline__ s32 hasBit(s32 mask, s32 bit) {
    return mask & bit;
}

DEFERRED void MantidQueen_Draw(u8 *actor, u8 *state, CDrawArgs *req) {
    WaveBossState *s;
    CGameObjectInstance *boss;
    s32 mask;
    Slot00403660 *slot;
    s32 i;
    s32 bit;

    i = 0;
    bit = 1;
    s = &gMantidQueen;
    boss = (CGameObjectInstance *) s->actor;
    mask = boss->m_AI.m_VisBits;
    slot = (Slot00403660 *) &s->spots[0].anim;
    do {
        if (hasBit(mask, bit << i)) {
            req->m_nModel = i;
            req->m_nShadowModel = i;
            CGameObjectInstance__DrawShadow(actor, state, req);
            ScriptAnimator_Apply(slot);
            ScreenTint_UpdateMode();
            /* the actor's draw matrices, lights and texture by member (N64 0x98, 0x110, 0x151) */
            Render_DrawModel(req->m_hModel, ((CGameObjectInstance *) actor)->m_pmtDrawMtxs, 1,
                             (char *) &((CGameObjectInstance *) actor)->m_Lights[gEvenOdd], 0, i,
                             ((CGameObjectInstance *) actor)->m_AI.m_nTexture);
            gBlendState = 0;
        }
        i++;
        slot++;
    } while (i < 5);
    if (boss->m_AI.m_Mode != 2) {
        boss->m_CollisionYOffset = 0;
    }
}

DEFERRED s32 MantidQueen_GetTurnSide(s32 a0, s32 a1, s32 a2) {
    s32 result;

    if (gMantidQueen.blink != 0) {
        result = AI_IsAimedAt((void *) a0, (void *) a1, a2, 0.785398245f);
    } else {
        result = 0;
    }
    return result;
}

extern void AI_ComputeFrameMotion(CGameObjectInstance *actor, CAI *ai, CAIFrameMotion *path);
extern f32 func_00298470(f32); /* sqrtf */

void MantidQueen_ChooseAttack(CGameObjectInstance *actor, CAI *ai) {
    CAITargets t;
    CAIFrameMotion path;
    CVector3 v;
    WaveBossStep *step;
    WaveBossState *boss = &gMantidQueen;
    f32 turn;
    sptr side;
    sptr visible;

    step = boss->step;
    actor->m_dwFlags |= 0x2000;
    turn = 0.17453295f;
    if (boss->waveActive) {
        turn = 0.3490659f;
    }
    AI_UpdateTargets(actor, ai, &t, 1, 1);
    AI_ComputeFrameMotion(actor, ai, &path);
    AI_TurnClamped(actor, (sptr) ai, turn, t.m_Path.m_Angle);
    visible = t.m_Sight.m_pTarget;
    if (gMantidQueen_blink == 0) {
        side = 0;
    } else {
        side = AI_IsAimedAt(actor, ai, visible, 0.785398245f);
    }
    if (side == -1) {
        CAI__SetMode(actor, ai, 11);
    } else if (side == 1) {
        CAI__SetMode(actor, ai, 12);
    } else if (boss->waveActive) {
        v.x = actor->header.m_vPos.x;
        v.y = 0.0f;
        v.z = actor->header.m_vPos.z;
        if (func_00298470(v.x * v.x + v.z * v.z) < 204.79999f) {
            CAI__SetMode(actor, ai, 4);
        }
    } else if (t.m_Path.m_pTarget && t.m_Sight.m_pTarget) {
        if (t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle <= 0.52359885f : t.m_Sight.m_Angle <= 0.52359885f) {
            if (t.m_Path.m_Dist > 1228.7999f && step->unkC) {
                CAI__SetMode(actor, ai, 9);
            } else if (t.m_Path.m_Dist < 921.6f) {
                CAI__SetMode(actor, ai, 10);
            }
        }
    }
}

void MantidQueen_Step(); /* defined later on its own view of the actor */
void EnemyAI_HeadNodeCallback(void);

void MantidQueen_InitAI(sptr arg0, CAI *arg1) {
    gScene_m_LevelID = 0x1D4D;
    arg1->m_pModeTable = (CAIModeInfo *) sMantidQueenModeTable;
    arg1->m_pfStep = MantidQueen_Step;
    arg1->m_pfHurt = MantidQueen_Hurt;
    arg1->m_pfEffects = &EnemyAI_HeadNodeCallback;
    arg1->m_pfDraw = MantidQueen_Draw;
    arg1->m_VisBits = -1;
    CAI__SetMode(arg0, arg1, 0);
}

typedef CVector3 Vec3_004028F4;
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* the view at the host offsets (round 9 x2, the Mantid Queen fight): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
typedef struct {
    union {
        struct { u8 _hp_rot[__builtin_offsetof(CGameObjectInstance, m_vVelocity)]; Vec3_004028F4 rot; };
    };
} Obj004028F4;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3_004028F4 rot;
} Obj004028F4;
#endif

void GameObject_TraceMove(Obj004028F4 *obj, Vec3_004028F4 pos, Vec3_004028F4 *posp, s32 *out, s32 a5, s32 a6);

void MantidQueen_SpawnMinion(s32 a0, Obj004028F4 *obj, s32 a2, Vec3_004028F4 pos) {
    WaveBossState *s = &gMantidQueen;
    s32 index;
    sptr region;

    index = WeightedTable_PickStatic(s->step->partPicks);
    GameObject_TraceMove(obj, pos, &pos, &region, 0, 0);
    CInstancePool__Spawn(&gScene.m_InstancePool, -1, sMantidQueenMinions[index].model, s->intel[index], region, pos, 0, obj->rot,
                  sMantidQueenMinions[index].offset, 0);
}

typedef struct {
    f32 x, y, z;
} Vec3_00402A10;
typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ void (*onReset)(sptr a0, void *obj);
} Def00402A10;
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 pad0[0x30 - 0x4];
    /* 0x30 */ Def00402A10 *def;
    /* 0x34 */ s8 kind;
    /* 0x35 */ u8 pad1[0x88 - 0x35];
    /* 0x88 */ s32 unk88;
    /* 0x8C */ u8 pad2[0x9C - 0x8C];
    /* 0x9C */ Vec3_00402A10 pos;
    /* 0xA8 */ s32 angle;
    /* 0xAC */ Vec3_00402A10 prevPos;
    /* 0xB8 */ s32 prevAngle;
} Obj00402A10;
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define AI_TurnHeadToTarget AI_TurnHeadToTarget_i
#endif

void AI_TurnHeadToTarget(sptr a0, Obj00402A10 *obj, s32 a2);
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* Host body (round 9 x2): the CAI by member. The N64 view (Obj00402A10) is the CAI: the mode
 * row m_pModeCurrent at 0x30 (its update m_pfCallback at + 8), the mode at 0x34, m_pPathTarget at
 * 0x88, m_vInitialPos/m_pInitialRegion copied to m_vLeashPos/m_pLeashRegion. On the host it read
 * bytes of the mode table pointers, so the Queen's mode updates never ran: she stayed in mode 1
 * (the intro) for good, no wave was sent and no spot opened. */
void MantidQueen_Step(sptr a0, Obj00402A10 *obj) {
    CAI *ai = (CAI *) obj;

    ai->m_vLeashPos = ai->m_vInitialPos;
    ai->m_pLeashRegion = ai->m_pInitialRegion;
    if (gMantidQueen.waveActive != 0 || ai->m_Mode == 15) {
        ai->m_pPathTarget = NULL;
        ai->m_dwFlags |= 0x20000000;
    } else {
        ai->m_dwFlags &= ~0x20000000;
    }
    AI_TurnHeadToTarget(a0, obj, 0);
    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ai->m_pModeCurrent->m_pfCallback(a0, ai);
    }
    MantidQueen_Update(&gMantidQueen);
}
#else
void MantidQueen_Step(sptr a0, Obj00402A10 *obj) {
    obj->prevPos = obj->pos;
    obj->prevAngle = obj->angle;
    if (gMantidQueen.waveActive != 0 || obj->kind == 15) {
        obj->unk88 = 0;
        obj->flags |= 0x20000000;
    } else {
        obj->flags &= ~0x20000000;
    }
    AI_TurnHeadToTarget(a0, obj, 0);
    if (obj->def != NULL && obj->def->onReset != NULL) {
        obj->def->onReset(a0, obj);
    }
    MantidQueen_Update(&gMantidQueen);
}
#endif
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* the view at the host offsets (round 9 x2, the Mantid Queen fight): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
typedef struct {
    union {
        struct { u8 _hp_onEvent[__builtin_offsetof(CGameObjectInstance, m_AI.m_pfDraw)]; void * onEvent; };
        struct { u8 _hp_onUpdate[__builtin_offsetof(CGameObjectInstance, m_AI.m_pfEffects)]; void * onUpdate; };
    };
} Actor00402ADC;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x234];
    /* 0x234 */ void *onEvent;
    /* 0x238 */ u8 pad1[0x248 - 0x238];
    /* 0x248 */ void *onUpdate;
} Actor00402ADC;
#endif

void MantidQueen_EnterStart(sptr a0, sptr a1) {
    Actor00402ADC *found[2];
    Actor00402ADC *actor;
    s32 count;
    s32 i;

    count = CScene__FindInstances(&gScene, 0x451, 9, -1, found, 2);
    for (i = 0; i < count; i++) {
        actor = found[i];
        actor->onEvent = MantidQueen_Draw;
        actor->onUpdate = EnemyAI_HeadNodeCallback;
    }
    MantidQueen_Init(&gMantidQueen, a0);
    CAI__SetMode(a0, a1, 1);
}

void MantidQueen_EnterIntro(void) {
    Cinema_RequestById(0x12C);
}
