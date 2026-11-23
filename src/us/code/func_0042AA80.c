/* WRONG CUT: REAL CScene__LoadLightSettings size 0x18C (splat cut it into CScene__LoadLightSettings + func_0042AB2C). */
#include "common.h"
#include "structs/world.h"
#include "structs/light.h"

/* The level's light settings (colours as bytes, direction as signed bytes). */


extern f32 func_00298470(f32);
extern void CVector3__Scale(f32 *out, f32 *in, f32 scale);

/* Loads the level's light colours into the scene light and its direction, normalised to 127, into the menu. */
void CScene__LoadLightSettings(CScene *menu) {
    s32 i;
    f32 dir[3];
    f32 len;

    for (i = 0; i < 3; i++) {
        gSceneLights.m_AmbColCopy[i] = gSceneLights.m_AmbCol[i] = menu->m_pLevelHeader->m_AmbientColor[i];
        gSceneLights.m_ColCopy[i] = gSceneLights.m_Col[i] = menu->m_pLevelHeader->m_LightColor[i];
        gSceneLights.m_Dir[i] = menu->m_pLevelHeader->m_LightDir[i];
    }
    dir[0] = menu->m_pLevelHeader->m_LightDir[0];
    dir[1] = menu->m_pLevelHeader->m_LightDir[1];
    dir[2] = menu->m_pLevelHeader->m_LightDir[2];
    len = func_00298470(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
    if (len != 0.0f) {
        CVector3__Scale(dir, dir, 127.0f / len);
        menu->m_LightDirection[0] = dir[0];
        menu->m_LightDirection[1] = dir[1];
        menu->m_LightDirection[2] = dir[2];
    }
}
