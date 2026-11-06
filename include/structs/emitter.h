#ifndef STRUCTS_EMITTER_H
#define STRUCTS_EMITTER_H

/* The region emitters: textured layers a region turns on by the bits of its
 * CRegionInfo.m_EmitterMask (one bit per entry of the definition table
 * D_8010FF14, D_8010FF18 entries). Each running emitter is drawn as a
 * translucent fan over the camera's view corners (CCamera.m_vTCorners) at
 * its height, which follows the camera, the floor or the water surface
 * (CEmitterPool__Advance updates them, CEmitter__Draw draws one, CEmitterPool__Draw sorts
 * them around the camera's height). A camera keeps four of them in its
 * CEmitterPool (CCamera.m_SkySystem, built by CEmitterPool__Construct).
 * Rage Wars has the same code (src/SCHED/8023A284_*.CPP) with unnamed types.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"
#include "structs/list.h"

/* A vertex as the RSP reads it. */
typedef struct UnitVtx {
    /* 0x0 */ s16 x, y, z;
    /* 0x6 */ s16 flag;
    /* 0x8 */ s16 s, t;
    /* 0xC */ u8 r, g, b, a;
} UnitVtx; /* size 0x10 */

/* CEmitterDef.m_Flags: what the height follows besides m_BaseHeight. */
#define EMITTER_DEF_FLAG_CAMERA 0x01 /* the camera's height * m_CameraScale */
#define EMITTER_DEF_FLAG_FLOOR  0x02 /* the floor under the camera */

/* One entry of the definition table D_8010FF14. */
typedef struct CEmitterDef {
    /* 0x00 */ s16 m_Texture;               /* 0: unused entry */
    /* 0x02 */ u8 m_Red;
    /* 0x03 */ u8 m_Green;
    /* 0x04 */ u8 m_Blue;
    /* 0x05 */ u8 m_EnvRed;
    /* 0x06 */ u8 m_EnvGreen;
    /* 0x07 */ u8 m_EnvBlue;
    /* 0x08 */ f32 m_SpinRate;              /* texture scroll speed */
    /* 0x0C */ f32 m_TextureScale;
    /* 0x10 */ f32 m_PhaseRate;             /* texture animation speed */
    /* 0x14 */ f32 m_BaseHeight;
    /* 0x18 */ f32 m_FadeDepth;
    /* 0x1C */ f32 m_CameraScale;
    /* 0x20 */ f32 m_ThrottleRate;          /* fade in / out speed */
    /* 0x24 */ u8 _pad24[4];
    /* 0x28 */ u8 m_Alpha;
    /* 0x29 */ u8 m_Flags;                  /* EMITTER_DEF_FLAG_* */
    /* 0x2A */ u8 _pad2A[2];
} CEmitterDef; /* size 0x2C */

/* A running emitter (an element of CEmitterPool's lists). */
typedef struct CEmitter {
    /* 0x000 */ struct CEmitter *m_pLast;
    /* 0x004 */ struct CEmitter *m_pNext;
    /* 0x008 */ u8 _pad8[4];
    /* 0x00C */ CEmitterDef *m_pDef;
    /* 0x010 */ u16 m_Mask;                 /* its bit of the region's m_EmitterMask */
    /* 0x012 */ u8 _pad12[2];
    /* 0x014 */ s32 m_Texture[2];           /* the texture handle (func_00207914) */
    /* 0x01C */ f32 m_ScrollS;              /* texture offsets, wrap at 20480 */
    /* 0x020 */ f32 m_ScrollT;
    /* 0x024 */ u8 _pad24[4];
    /* 0x028 */ UnitVtx m_Vertices[2][15];  /* one fan per frame buffer */
    /* 0x208 */ f32 m_Throttle;             /* 0..1, fades in while its region asks for it */
    /* 0x20C */ f32 m_Phase;                /* texture animation frame */
    /* 0x210 */ f32 m_Height;
    /* 0x214 */ f32 m_PhaseScale;
    /* 0x218 */ f32 m_TextureScale;
    /* 0x21C */ f32 m_HeightOffset;
    /* 0x220 */ f32 m_Opacity;
    /* 0x224 */ u8 _pad224[4];
} CEmitter; /* size 0x228 */

typedef struct CEmitterPool {
    /* 0x000 */ CEmitter m_Emitters[4];
    /* 0x8A0 */ CList m_FreeList;
    /* 0x8B4 */ CList m_ActiveList;
    /* 0x8C8 */ u16 m_ActiveMask;           /* the definition bits running */
    /* 0x8CA */ u8 _pad8CA[6];
} CEmitterPool; /* size 0x8D0 */

#endif
