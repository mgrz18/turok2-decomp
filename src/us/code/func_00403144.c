#include "common.h"
#include "globals/boss.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"
/* the view at the host offsets of CAITargets (soak S7); unk4 is the sight target (a pointer) */
typedef struct {
    union {
        struct { s32 unk0; };
        struct { u8 _hp_unk4[__builtin_offsetof(CAITargets, m_Sight.m_pTarget)]; sptr unk4; };
        struct { u8 _hp_unk4C[__builtin_offsetof(CAITargets, m_Path.m_Angle)]; s32 unk4C; }; /* N64 0x4C */
        u8 _hp_whole[sizeof(CAITargets)];
    };
} Info00403144;
#else
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad0[0x4C - 0x8];
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ u8 pad1[0x88 - 0x50];
} Info00403144;
#endif

#ifdef TARGET_PORT
/* the CAI's m_AnimRepeatedCount at its host offset (round 10 y2): the N64 0xC7 read another byte,
 * the turn never counted its 4 repeats and the Queen turned (mode 11) for good once the full mode
 * table let her reach it */
typedef struct {
    union {
        struct { u8 _hp_count[__builtin_offsetof(CAI, m_AnimRepeatedCount)]; s8 count; }; /* N64 0xC7 */
    };
} Item00403144;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0xC7];
    /* 0xC7 */ s8 count;
} Item00403144;
#endif

void AI_UpdateTargets(sptr a0, Item00403144 *item, Info00403144 *info, s32 a3, s32 a4);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define AI_TurnClamped AI_TurnClamped_i
#endif
void AI_TurnClamped(sptr a0, Item00403144 *item, f32 a2, s32 a3);
s32 AI_IsAimedAt(sptr a0, Item00403144 *item, sptr a2, f32 a3);
void CAI__SetMode(sptr a0, Item00403144 *item, s32 a2);

static __inline__ s32 func_00403774_inline(sptr a0, Item00403144 *item, sptr a2) {
    s32 result;

    if (gMantidQueen.blink == 0) {
        result = 0;
    } else {
        result = AI_IsAimedAt(a0, item, a2, 0.785398245f);
    }
    return result;
}

void MantidQueen_UpdateTurn(sptr a0, Item00403144 *item) {
    Info00403144 info;
    s32 side;

    AI_UpdateTargets(a0, item, &info, 1, 1);
    AI_TurnClamped(a0, item, 0.17453295f, info.unk4C);
    side = func_00403774_inline(a0, item, info.unk4);
    if (side == -1) {
        CAI__SetMode(a0, item, 11);
    } else if (side == 1) {
        CAI__SetMode(a0, item, 12);
    } else if (item->count >= 4) {
        CAI__SetMode(a0, item, 8);
    }
}
