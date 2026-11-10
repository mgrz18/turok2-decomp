/* WRONG CUT: REAL CScene__LoadBitSections2 size 0x4C (splat cut it into CScene__LoadBitSections2 + func_0042C480). */
#include "common.h"
#include "globals/scene.h"
#include "globals/sysrc.h"


extern s32 SavedPlayers_IsActive(void);
extern void CLoader__LoadSync(CLoader *reader, sptr, sptr, void *);

typedef struct MenuState {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[4];
    /* 0x34 */ s32 unk34;
} MenuState;

void CScene__LoadBitSections2(MenuState *menu) {
    if (SavedPlayers_IsActive() == 0) {
        CLoader__LoadSync(&gLoader, menu->unk2C, menu->unk34, gGamePersistentData);
    }
}
