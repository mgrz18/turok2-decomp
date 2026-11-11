#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif

/* Picks the death animation for an AI actor (modes 0x33/0x34): a special one
 * when the region under it is flagged, then the explosive, violent, moving or
 * normal death pickers (the m_w*DeathAnimFlags tables) allowed by the mode
 * bits (20-25), the index (0..2) choosing which 5-bit group of the def's
 * death-flag words enables it. Not an attack picker, as this comment once
 * said. */

#ifdef TARGET_PORT
typedef struct AttackActor {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_AttackActor(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_target[T2_HOFF_AttackActor(header.m_pCurrentRegion)]; sptr target; }; /* N64 0x10: the region pointer, pointer-wide */
        struct { u8 _hp_def[T2_HOFF_AttackActor(header.m_pIntelligence)]; s32 *def; }; /* N64 0x14 */
    };
} AttackActor;
#else
typedef struct {
    /* 0x00 */ u8 pad00[0x10];
    /* 0x10 */ s32 target;
    /* 0x14 */ s32 *def;
} AttackActor;
#endif

#ifdef TARGET_PORT
typedef struct AttackState {
/* the view at the host offsets of CAI (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_AttackState(m) __builtin_offsetof(CAI, m)
    union {
        struct { u8 _hp_flags[T2_HOFF_AttackState(m_dwFlags)]; s32 flags; }; /* N64 0x0 */
        struct { u8 _hp_anim[T2_HOFF_AttackState(m_AnimType)]; s16 anim; }; /* N64 0xC4 */
        struct { u8 _hp_index[T2_HOFF_AttackState(m_Enemy.m_BehaviorIndex)]; s8 index; }; /* N64 0x133 (round 8 w2: by hand, inside the m_VarBuffer union) */
    };
} AttackState;
#else
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 pad04[0xC0];
    /* 0xC4 */ s16 anim;
    /* 0xC6 */ u8 padC6[0x6D];
    /* 0x133 */ s8 index;
} AttackState;
#endif

typedef struct {
    /* 0x00 */ u8 pad00[0x44];
    /* 0x44 */ s32 flags;
    /* 0x48 */ u8 pad48[0xA];
    /* 0x52 */ u16 flags2;
} TargetInfo;

sptr CGameObjectInstance__PickRandomAnim(void *, sptr, sptr);
s32 EnemyAnim_NormalDeath_B0120(void *, void *);
s32 EnemyAnim_NormalDeath_B0150(void *, void *);
s32 EnemyAnim_NormalDeath_B0180(void *, void *);
s32 EnemyAnim_MovingDeath_B01B0(void *, void *);
s32 EnemyAnim_MovingDeath_B01E0(void *, void *);
s32 EnemyAnim_MovingDeath_B0210(void *, void *);
s32 EnemyAnim_ViolentDeath_B0240(void *, void *);
s32 EnemyAnim_ViolentDeath_B0270(void *, void *);
s32 EnemyAnim_ViolentDeath_B02A0(void *, void *);
s32 EnemyAnim_ExplosiveDeath_B02D0(void *, void *);
s32 EnemyAnim_ExplosiveDeath_B0300(void *, void *);
s32 EnemyAnim_ExplosiveDeath_B0330(void *, void *);
s32 EnemyAnim_NormalDeath_B0360(void *, void *);

s32 EnemyAnim_PickDeath(AttackActor *actor, AttackState *state) {
    s32 result = -1;
    s32 flags = state->flags;
    s32 *def = actor->def;
    s32 index = state->index;
    s32 mode = flags & 0x03F00000;
    sptr target = actor->target;
    u16 *masks = (u16 *)((u8 *)def + 0x14); /* [0x3C] close, [0x3D] mid, [0x3E] far */
    s32 rest = flags & ~0x03F00000;
    TargetInfo *t;

    if (*def != 1) {
        return 0;
    }
    if (target != 0) {
        t = CScene__GetRegionInfo(&gScene, target);
        if (t != NULL && ((t->flags & 0x400000) || (t->flags2 & 0xC0))) {
            result = EnemyAnim_NormalDeath_B0360(actor, state);
            if (result != -1) {
                return result;
            }
        }
    }
    if (mode & 0xC00000) {
        if (index == 0 && (masks[0x3E] & 0x1F)) {
            result = EnemyAnim_ExplosiveDeath_B02D0(actor, state);
        } else if (index == 1 && (masks[0x3E] & 0x3E0)) {
            result = EnemyAnim_ExplosiveDeath_B0300(actor, state);
        } else if (index == 2 && (masks[0x3E] & 0x7C00)) {
            result = EnemyAnim_ExplosiveDeath_B0330(actor, state);
        }
        if (CGameObjectInstance__PickRandomAnim(actor, result, -1) == -1) {
            mode = 0x200000;
        }
    }
    if (mode & 0x200000) {
        if (index == 0 && (masks[0x3D] & 0x1F)) {
            result = EnemyAnim_ViolentDeath_B0240(actor, state);
        } else if (index == 1 && (masks[0x3D] & 0x3E0)) {
            result = EnemyAnim_ViolentDeath_B0270(actor, state);
        } else if (index == 2 && (masks[0x3D] & 0x7C00)) {
            result = EnemyAnim_ViolentDeath_B02A0(actor, state);
        }
        if (CGameObjectInstance__PickRandomAnim(actor, result, -1) == -1) {
            mode = 0x100000;
        }
    }
    if (result == -1) {
        if (index == 0) {
            if ((masks[0x3C] & 0x1F) && state->anim >= 2000 && state->anim < 2025) {
                result = EnemyAnim_MovingDeath_B01B0(actor, state);
            } else {
                result = EnemyAnim_NormalDeath_B0120(actor, state);
            }
        } else if (index == 1) {
            if ((masks[0x3C] & 0x3E0) && state->anim >= 2100 && state->anim < 2125) {
                result = EnemyAnim_MovingDeath_B01E0(actor, state);
            } else {
                result = EnemyAnim_NormalDeath_B0150(actor, state);
            }
        } else if (index == 2) {
            if ((masks[0x3C] & 0x7C00) && state->anim >= 2200 && state->anim < 2225) {
                result = EnemyAnim_MovingDeath_B0210(actor, state);
            } else {
                result = EnemyAnim_NormalDeath_B0180(actor, state);
            }
        }
    }
    state->flags = rest | mode;
    return result;
}
