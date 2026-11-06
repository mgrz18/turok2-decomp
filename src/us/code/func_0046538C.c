#include "common.h"

s32 CLoader__LoadSync(s32, s32, s32, s32);

s32 SavedPlayers_IsActive();                                /* extern */
extern s32 gLoader;
extern s32 gGamePersistentData;

typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[4];
    /* 0x34 */ s32 unk34;
} Unk0046538C;

void CScene__LoadBitSections2_2(Unk0046538C *arg0) {
    if (SavedPlayers_IsActive() == 0) {
        CLoader__LoadSync((s32) &gLoader, arg0->unk2C, arg0->unk34, (s32) &gGamePersistentData);
    }
}
