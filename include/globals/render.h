#ifndef GLOBALS_RENDER_H
#define GLOBALS_RENDER_H

/* The screen, the video modes and the frame buffers (gDList is gfx.h's,
 * gVideoModes / gVideoModesPAL frame.h's). */

#include "common.h"
#include "gfx.h"
#include "structs/light.h"

extern s32 gScreenWidth;            /* Turok 3's screen_width */
extern s32 gScreenHeight;           /* Turok 3's screen_height */
extern s32 gResolution;             /* the resolution set, index in gVideoModes */
extern s32 gAppliedResolution;      /* the resolution last applied */
extern s32 gVideoClearFrames;       /* frames left to clear the screen around a mode change */
extern s32 gVideoModeCounts[2];     /* resolutions per TV region */
extern u32 gTotalFramebuffers;      /* 3 */
extern u32 gDisplayListSize;        /* in commands */
extern u32 gLineListSize;           /* in commands */
extern u32 gNextFrameToShow;        /* wraps at gTotalFramebuffers */
extern s32 gDLightBufferIndex;       /* the half of gDLightBuffer in use (DLights_SwapBuffers) */

/* Render state: the init display lists, the texture/shadow display lists,
 * the cached combine/render modes, the video-mode render flags, fog and the
 * prim colour overrides. gSceneLights and the blend override (gBlendState,
 * gBlendState_m_Color) are in structs/light.h. */

extern CLights1 gDefaultLights;     /* the lights of an instance drawn with lit vertices, of menu models */

/* Called first by every display list: RSP (geometry mode, lights) and RDP (othermode) defaults. */
extern Gfx gRspInitDL[6];
extern Gfx gRdpInitDL[12];

/* Texture_Load: by detail mode / wrap / format. */
extern Gfx sTextureLoadDL_Detail[16];
extern Gfx sTextureLoadDL_DetailMirror[16];
extern Gfx sTextureLoadDL_Mode2[19];
extern Gfx sTextureLoadDL_Mode3[19];
extern Gfx sTextureLoadDL_I16[19];
extern s32 sLoadedTexture;          /* the texture last loaded (0: none) */

/* Render_DrawShadowQuad: pass 0 / other passes, and its 16x16 I8 texture. */
extern Gfx sShadowQuadDL[17];
extern Gfx sShadowQuadDL2[18];
extern u8 gShadowTexture[16 * 16];

extern s32 gCurrentCombineMode;     /* cached Render_SetCombineMode index, -1 none */
extern s32 gCurrentRenderMode;      /* cached Render_SetRenderMode index, -1 none */

/* The video mode flags (Video_ApplyResolution). */
extern s32 gRenderAntiAlias;        /* the AA render modes */
extern s32 gViAntiAlias;            /* the AA VI mode (Video_SetMode) */
extern s32 gRenderZModeInter;       /* ZMODE_INTER render modes */
extern s32 gRenderImageRead;        /* IM_RD render modes */

/* Fog (func_0020B554 derives the three masks from gFogEnabled). */
extern s32 gFogEnabled;
extern u32 gFogGeometryMode;        /* 0 or G_FOG */
extern u32 gFogRenderModeFogA;      /* blender cycle 1 bits */
extern u32 gFogRenderModeShadeA;    /* blender cycle 1 bits of the z-buffered modes */

/* Render_SetupMaterial: the prim colour / alpha overrides. */
extern u8 gPrimColorOverride;
extern u8 gPrimColorOverrideR;
extern u8 gPrimColorOverrideG;
extern u8 gPrimColorOverrideB;
extern u8 gPrimAlphaOverride;
extern u8 gPrimAlphaOverrideValue;

#endif
