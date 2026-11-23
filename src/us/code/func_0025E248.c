#include "common.h"
#include "protos/ai.h"
#include "structs/game_object.h"

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


sptr CGameObjectInstance__PickRandomAnim(void *, sptr, sptr);
s32 EnemyAnim_NormalDeath_B0390(void *, void *);
s32 EnemyAnim_NormalDeath_B03C0(void *, void *);
s32 GameObject_IsOnGround(void *);
void AI_DropPickupsOnce(void *, void *, s32, s32, s32);
void AI_StopSound(void *, void *);
void CListedInstance__Free(void *);

void EnemyMode_Death_Update(AttackActor *actor, CAI *state) {
    AttackInfo *info = (AttackInfo *)(actor->def + 0x14);
    s32 anim;

    if (state->m_Enemy.m_BehaviorIndex == 2) {
        switch (state->m_AnimType) {
        case 7020:
        case 7025:
        case 7030:
        case 7035:
        case 7040:
            if (state->m_AnimRepeatedCount == 0) {
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
        state->m_AnimIndex = CGameObjectInstance__PickRandomAnim(actor, anim, -1);
        state->m_AnimType = anim;
        state->m_AnimRepeatedCount = 0;
        state->m_dwFlags &= ~1;
        return;
    }
finish:
    if (state->m_AnimRepeatedCount != 0) {
        if (state->m_Mode == 0x35) {
            CAI__SetMode(actor, state, 0x36);
        } else {
            state->m_dwFlags |= 0x20;
            actor->flags &= ~0x01000000;
            AI_DropPickupsOnce(actor, state, 4, info->unk0C, info->unk10);
            actor->flags &= ~0x20000;
            if (state->m_ModeTime > 90.0f) {
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
        state->m_ModeTime = 0.0f;
    }
}
