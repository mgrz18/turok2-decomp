#include "common.h"
#include "protos/ai.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif

/* Index-2 attack follow-up: in the 7020-7041 animation band it chains to the
 * next move; otherwise it finishes the current one. */

typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ u8 pad08[4];
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
} AttackInfo;

#ifdef TARGET_PORT
typedef struct AttackActor {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_AttackActor(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_def[T2_HOFF_AttackActor(header.m_pIntelligence)]; u8 *def; }; /* N64 0x14 */
        struct { u8 _hp_flags[T2_HOFF_AttackActor(m_dwFlags)]; s32 flags; }; /* N64 0xD4 */
    };
} AttackActor;
#else
typedef struct {
    /* 0x00 */ u8 pad00[0x14];
    /* 0x14 */ u8 *def;
    /* 0x18 */ u8 pad18[0xBC];
    /* 0xD4 */ s32 flags;
} AttackActor;
#endif

#ifdef TARGET_PORT
typedef struct AttackState {
/* the view at the host offsets of CAI (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_AttackState(m) __builtin_offsetof(CAI, m)
    union {
        struct { u8 _hp_flags[T2_HOFF_AttackState(m_dwFlags)]; s32 flags; }; /* N64 0x0 */
        struct { u8 _hp_move[T2_HOFF_AttackState(m_Mode)]; s8 move; }; /* N64 0x34 */
        struct { u8 _hp_time[T2_HOFF_AttackState(m_ModeTime)]; f32 time; }; /* N64 0x40 */
        struct { u8 _hp_anim[T2_HOFF_AttackState(m_AnimType)]; s16 anim; }; /* N64 0xC4 */
        struct { u8 _hp_animSlot[T2_HOFF_AttackState(m_AnimIndex)]; s8 animSlot; }; /* N64 0xC6 */
        struct { u8 _hp_queued[T2_HOFF_AttackState(m_AnimRepeatedCount)]; s8 queued; }; /* N64 0xC7 */
        struct { u8 _hp_index[T2_HOFF_AttackState(m_Enemy.m_BehaviorIndex)]; s8 index; }; /* N64 0x133 (round 8 w2: by hand, inside the m_VarBuffer union) */
    };
} AttackState;
#else
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 pad04[0x30];
    /* 0x34 */ s8 move;
    /* 0x35 */ u8 pad35[0xB];
    /* 0x40 */ f32 time;
    /* 0x44 */ u8 pad44[0x80];
    /* 0xC4 */ s16 anim;
    /* 0xC6 */ s8 animSlot;
    /* 0xC7 */ s8 queued;
    /* 0xC8 */ u8 padC8[0x6B];
    /* 0x133 */ s8 index;
} AttackState;
#endif

sptr CGameObjectInstance__PickRandomAnim(void *, sptr, sptr);
s32 EnemyAnim_NormalDeath_B0390(void *, void *);
s32 EnemyAnim_NormalDeath_B03C0(void *, void *);
s32 GameObject_IsOnGround(void *);
void AI_DropPickupsOnce(void *, void *, s32, s32, s32);
void AI_StopSound(void *, void *);
void CListedInstance__Free(void *);

void EnemyMode_Death_Update(AttackActor *actor, AttackState *state) {
    AttackInfo *info = (AttackInfo *)(actor->def + 0x14);
    s32 anim;

    if (state->index == 2) {
        switch (state->anim) {
        case 7020:
        case 7025:
        case 7030:
        case 7035:
        case 7040:
            if (state->queued == 0) {
                return;
            }
            anim = EnemyAnim_NormalDeath_B0390(actor, state);
            break;
        case 7021:
        case 7026:
        case 7031:
        case 7036:
        case 7041:
            if (!GameObject_IsOnGround(actor)) {
                return;
            }
            anim = EnemyAnim_NormalDeath_B03C0(actor, state);
            break;
        default:
            goto finish;
        }
        state->animSlot = CGameObjectInstance__PickRandomAnim(actor, anim, -1);
        state->anim = anim;
        state->queued = 0;
        state->flags &= ~1;
        return;
    }
finish:
    if (state->queued != 0) {
        if (state->move == 0x35) {
            CAI__SetMode(actor, state, 0x36);
        } else {
            state->flags |= 0x20;
            actor->flags &= ~0x01000000;
            AI_DropPickupsOnce(actor, state, 4, info->unk0C, info->unk10);
            actor->flags &= ~0x20000;
            if (state->time > 90.0f) {
                CAI__SetMode(actor, state, 0x3C);
            }
        }
        if (info->flags & 0x10000) {
            actor->flags &= ~0x100;
            actor->flags &= ~0x2000;
            AI_StopSound(actor, state);
            if (actor->flags & 0x80000) {
                CListedInstance__Free(actor);
            }
        }
    } else {
        state->time = 0.0f;
    }
}
