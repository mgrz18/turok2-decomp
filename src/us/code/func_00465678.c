#include "common.h"
#include "structs/world.h"

s32 CScene__LoadLightSettings_2();                            /* extern */
extern s32 D_800AB8A0;

void CScene__SetupLighting_2(CScene *arg0) {
    CScene__LoadLightSettings_2();
    D_800AB8A0 = ((u8) arg0->m_pLevelHeader->m_Flags >> 2) & 1;
}
