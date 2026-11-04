#include "common.h"
#include "structs/player.h"
#include "structs/game.h"



s32 func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);
s32 func_00248BA8(s32, s32, s32, s32);

CPlayer *func_002532A8(CPlayerList *);                     /* extern */
extern CGameObjectInstance *D_8012FB70;

void func_0040C7DC(void) {
    CPlayer *temp_v0;
    GameSettings *temp_v1;

    func_00243414(D_8012FB70, &D_8012FB70->m_AI, 1);
    temp_v0 = func_002532A8(&D_80119870.m_PlayerList);
    if (temp_v0 != NULL) {
        temp_v1 = &D_80119870.m_Settings;
        temp_v0->m_Data.m_bMotherBossComplete = 1;
        temp_v1->unlockedCheats = (s32) (temp_v1->unlockedCheats | 4);
        func_00248BA8((s32) temp_v0, 0x1B57, 2, 0);
    }
}
