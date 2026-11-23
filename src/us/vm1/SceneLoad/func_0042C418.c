#include "common.h"
#include "globals/scene.h"
#include "globals/sysrc.h"

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 done;
    /* 0x08 */ u8 pad8[0x28 - 0x8];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[4];
    /* 0x30 */ s32 unk30;
} Arg0;

s32 CLoader__LoadSync(CLoader *reader, sptr, sptr, sptr);
s32 SavedPlayers_IsActive();                                /* extern */

void CScene__LoadBitSections(Arg0 *arg0) {
    if (SavedPlayers_IsActive() == 0) {
        CLoader__LoadSync(&gLoader, arg0->unk28, arg0->unk30, (sptr) &gLoadSavePersistentData);
        arg0->done = 1;
    }
}
