#include "common.h"
#include "structs/game_object.h"

#ifdef TARGET_PORT
/* the view at the host offsets of CAITargets (soak S7: AI_UpdateTargets wrote the host CAITargets
 * over a 0x88-byte local and the caller's saved obj/ai: a crash in the next line) */
typedef struct {
    union {
        struct { u8 _hp_unk8[__builtin_offsetof(CAITargets, m_Sight.m_Angle)]; s32 unk8; }; /* N64 0x08 */
        u8 _hp_whole[sizeof(CAITargets)];
    };
} TargetInfo;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x88 - 0xC];
} TargetInfo; /* size 0x88 */
#endif

void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, TargetInfo *info, s32 a, s32 b);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define AI_TurnClamped AI_TurnClamped_i
#endif
void AI_TurnClamped(CGameObjectInstance *obj, CAI *ai, f32 angle, s32 unk);
void CPathTrack__Advance(CPathTrack *track);
void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

void EnemyMode_InteractiveIdle_Update(CGameObjectInstance *obj, CAI *ai) {
    TargetInfo info;
    f32 angle;

    AI_UpdateTargets(obj, ai, &info, 0, 1);
    angle = 0.2617994249f; /* 15 deg */
    if ((u8) ai->m_ModeBefore == 1 || (u8) ai->m_ModeBefore == 2) {
        angle = 1.0471977f; /* 60 deg */
    }
    AI_TurnClamped(obj, ai, angle, info.unk8);
    if (ai->m_AnimRepeatedCount != 0 || ai->m_Action == 1) {
        CPathTrack__Advance(&ai->m_PathTrack);
        CAI__SetMode(obj, ai, 10);
    }
}
