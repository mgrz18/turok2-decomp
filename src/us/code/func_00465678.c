#include "common.h"
#include "structs/world.h"

s32 func_0042A9A0();                            /* extern */
extern s32 D_800AB8A0;

void CScene__SetupLighting_2(CScene *arg0) {
    func_0042A9A0();
    D_800AB8A0 = ((u8) arg0->m_pLevelHeader->m_Flags >> 2) & 1;
}
