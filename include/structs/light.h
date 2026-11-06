#ifndef STRUCTS_LIGHT_H
#define STRUCTS_LIGHT_H

/* Lighting: the libultra light block the display lists load (Lights1: one
 * ambient and one directional light), the scene light D_800AB0D0 (the level's
 * colours and direction, CScene__LoadLightSettings; the default lights of a placed actor,
 * CLevelActor__Construct; what CScene__GetLightsAt returns where the level has no light
 * table), and the draw colour override D_800AB8B0 that Render_SetRenderMode applies
 * to every material (most files only write its mode, as an s32).
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

/* The draw colour override (D_800AB8B0), set by ScreenTint_UpdateMode from the
 * fade / alpha D_800AB8C0 accumulates. m_Mode: 0 none, 1 alpha, 3 fade to
 * m_Color, 6 both. */
typedef struct CBlendState {
    /* 0x00 */ s32 m_Mode;
    /* 0x04 */ CVector3 m_Color;            /* the env colour, 0..255 */
} CBlendState; /* size 0x10 */

extern CLights1 D_800AB0D0;     /* the scene light */
extern CBlendState D_800AB8B0;
extern f32 D_800AB8C0[2];       /* right after D_800AB8B0: [0] the fade (env alpha), [1] the alpha (prim alpha) */

#endif
