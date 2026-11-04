#include "common.h"
#include "structs/game_object.h"

extern s32 D_800B17C0;
extern s32 D_800B1840;
extern s32 D_800B18C0;

s32 func_0025EAB0(CGameObjectInstance *arg0, CAI *arg1) {
    s32 temp_v1;

    temp_v1 = (s32) arg1->m_pModeTable;
    if ((temp_v1 == (s32) &D_800B17C0) || (temp_v1 == (s32) &D_800B1840) || (temp_v1 == (s32) &D_800B18C0)) {
        return 0;
    }
    return (u16) arg0->m_nTypeFlag != 0x259;
}
