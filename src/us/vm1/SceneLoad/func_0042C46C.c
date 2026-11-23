/* WRONG CUT: REAL CScene__LoadBitSections2 size 0x4C (splat cut it into CScene__LoadBitSections2 + func_0042C480). */
#include "common.h"
#include "structs/world.h"
#include "globals/scene.h"
#include "globals/sysrc.h"


extern s32 SavedPlayers_IsActive(void);
extern void CLoader__LoadSync(CLoader *reader, sptr, sptr, void *);


void CScene__LoadBitSections2(CScene *menu) {
    if (SavedPlayers_IsActive() == 0) {
        CLoader__LoadSync(&gLoader, menu->unk_2C, menu->unk_34, gGamePersistentData);
    }
}
