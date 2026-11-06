/* WRONG CUT: REAL CScene__LoadLightSettings size 0x18C (splat cut it into CScene__LoadLightSettings + func_0042AB2C). */
#include "common.h"
#include "structs/light.h"

/* The level's light settings (colours as bytes, direction as signed bytes). */
typedef struct LevelLights {
    /* 0x00 */ u8 pad0[0xA];
    /* 0x0A */ u8 color[3];
    /* 0x0D */ u8 ambient[3];
    /* 0x10 */ u8 flags;
    /* 0x11 */ s8 dir[3];
} LevelLights;

typedef struct MenuState {
    /* 0x00000 */ u8 pad0[0x18E50];
    /* 0x18E50 */ LevelLights *lights;
    /* 0x18E54 */ s16 dir[3];
} MenuState;

extern f32 func_00298470(f32);
extern void CVector3__Scale(f32 *out, f32 *in, f32 scale);

/* Loads the level's light colours into the scene light and its direction, normalised to 127, into the menu. */
void CScene__LoadLightSettings(MenuState *menu) {
    s32 i;
    f32 dir[3];
    f32 len;

    for (i = 0; i < 3; i++) {
        gSceneLights.m_AmbColCopy[i] = gSceneLights.m_AmbCol[i] = menu->lights->ambient[i];
        gSceneLights.m_ColCopy[i] = gSceneLights.m_Col[i] = menu->lights->color[i];
        gSceneLights.m_Dir[i] = menu->lights->dir[i];
    }
    dir[0] = menu->lights->dir[0];
    dir[1] = menu->lights->dir[1];
    dir[2] = menu->lights->dir[2];
    len = func_00298470(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
    if (len != 0.0f) {
        CVector3__Scale(dir, dir, 127.0f / len);
        menu->dir[0] = dir[0];
        menu->dir[1] = dir[1];
        menu->dir[2] = dir[2];
    }
}
