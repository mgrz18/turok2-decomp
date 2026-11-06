#include "common.h"

s32 CScene__SetInstanceSavedBitInLevel(s32 *, u16, u16, s32); /* extern */
extern s32 D_800F7078;

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ u16 unk4;
    /* 0x6 */ u8 pad6[0xA - 0x6];
    /* 0xA */ u16 unkA;
} Unk00264900;

void Action_EventIdleRemote(Unk00264900 *arg0) {
    CScene__SetInstanceSavedBitInLevel(&D_800F7078, arg0->unkA, arg0->unk4, 0);
}
