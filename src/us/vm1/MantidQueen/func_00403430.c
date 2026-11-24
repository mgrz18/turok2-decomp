#include "common.h"
#include "protos/ai.h"

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



void MantidQueen_UpdateBossAnim(Owner00403430 *owner) {
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
