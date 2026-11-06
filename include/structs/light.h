#ifndef STRUCTS_LIGHT_H
#define STRUCTS_LIGHT_H

/* Lighting: the libultra light block the display lists load (Lights1: one
 * ambient and one directional light), the scene light gSceneLights (the level's
 * colours and direction, CScene__LoadLightSettings; the default lights of a placed actor,
 * CLevelActor__Construct; what CScene__GetLightsAt returns where the level has no light
 * table), and the draw colour override gBlendState / gBlendState_m_Color that
 * Render_SetupMaterial applies to every material.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"
#include "structs/vector.h"

/* libultra's Lights1 (PR/gbi.h: Ambient a; Light l[1]), the colours twice
 * (col / colc) as the RSP wants them. */
typedef struct CLights1 {
    /* 0x00 */ u8 m_AmbCol[3];
    /* 0x03 */ u8 _pad3;
    /* 0x04 */ u8 m_AmbColCopy[3];
    /* 0x07 */ u8 _pad7;
    /* 0x08 */ u8 m_Col[3];
    /* 0x0B */ u8 _padB;
    /* 0x0C */ u8 m_ColCopy[3];
    /* 0x0F */ u8 _padF;
    /* 0x10 */ s8 m_Dir[3];                 /* normalised to 127 */
    /* 0x13 */ u8 _pad13[5];
} __attribute__((aligned(4))) CLights1; /* size 0x18 */

extern CLights1 gSceneLights;     /* the scene light */
/* The draw colour override, set by ScreenTint_UpdateMode and applied by
 * Render_SetupMaterial to every material. Two plain variables, not a struct:
 * every file that reaches the mode through a struct (MEM_IN_STRUCT) schedules
 * differently and stops matching (EnemyAI_DrawFaded, CLevelActorPool__Draw...).
 * gBlendState: 0 none, 1 alpha, 3 fade to the colour, 6 both.
 * gBlendState_m_Color: the env colour RGB (0..255), the env alpha (fade), the prim alpha. */
extern s32 gBlendState;
extern f32 gBlendState_m_Color[5];
/* gBlendFade (0x800AB8C0) is &gBlendState_m_Color[3]: the C reaches it through
 * gBlendState_m_Color[3] / [4] (same code), so it has no declaration of its own. */

#endif
