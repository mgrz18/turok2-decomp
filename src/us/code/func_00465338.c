#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 done;
    /* 0x08 */ u8 pad8[0x28 - 0x8];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[4];
    /* 0x30 */ s32 unk30;
} Loader;

s32 CLoader__LoadSync(s32, s32, s32, s32);
s32 SavedPlayers_IsActive();                                /* extern */
extern s32 D_800D1CD8;
extern s32 D_800EBAC0;

void CScene__LoadBitSections_2(Loader *arg0) {
    if (SavedPlayers_IsActive() == 0) {
        CLoader__LoadSync((s32) &D_800D1CD8, arg0->unk28, arg0->unk30, (s32) &D_800EBAC0);
        arg0->done = 1;
    }
}
