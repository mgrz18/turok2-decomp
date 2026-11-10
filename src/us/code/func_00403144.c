#include "common.h"
#include "globals/boss.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad0[0x4C - 0x8];
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ u8 pad1[0x88 - 0x50];
} Info00403144;

typedef struct {
    /* 0x00 */ u8 pad0[0xC7];
    /* 0xC7 */ s8 count;
} Item00403144;

void AI_UpdateTargets(sptr a0, Item00403144 *item, Info00403144 *info, s32 a3, s32 a4);
void AI_TurnClamped(sptr a0, Item00403144 *item, f32 a2, s32 a3);
s32 AI_IsAimedAt(sptr a0, Item00403144 *item, sptr a2, f32 a3);
void CAI__SetMode(sptr a0, Item00403144 *item, s32 a2);

static __inline__ s32 func_00403774_inline(s32 a0, Item00403144 *item, s32 a2) {
    s32 result;

    if (gMantidQueen.blink == 0) {
        result = 0;
    } else {
        result = AI_IsAimedAt(a0, item, a2, 0.785398245f);
    }
    return result;
}

void MantidQueen_UpdateTurn(s32 a0, Item00403144 *item) {
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
