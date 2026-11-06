#include "common.h"
#include "structs/world.h"
#include "globals/render.h"

s32 CScene__LoadLightSettings_2();                            /* extern */

void CScene__SetupLighting_2(CScene *arg0) {
    CScene__LoadLightSettings_2();
    gFogEnabled = ((u8) arg0->m_pLevelHeader->m_Flags >> 2) & 1;
}
