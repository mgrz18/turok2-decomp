#ifndef GLOBALS_CAMERA_H
#define GLOBALS_CAMERA_H

/* The camera pool of the game, gEngineApp_m_Game.m_CameraPool (CGame + 0x40). */

#include "structs/camera.h"
#include "structs/collision.h"
#include "common.h"

extern CCameraPool gEngineApp_m_Game_m_CameraPool;

/* Members with symbols of their own (kept where the ROM bases the access on them). */
extern void *gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead;  /* .m_ActiveList.m_pHead */
extern s32 gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount;   /* .m_ActiveList.m_nCount */
extern CCamera gEngineApp_m_Game_m_CameraPool_m_MainCamera;        /* .m_MainCamera */

/* What the active cameras see (CCameraPool__Advance). */

extern CBox gCameraVisibleBox;      /* widened for animation */
extern CBox gCameraViewBox;         /* the plain view boxes */
extern u32 gCameraVisBits;          /* the visibility bits of the active views */
extern f32 gScreenClearTime;        /* seconds the screen is cleared for while the view moves */

/* The cameras' data, 0x800B6CB0-0x800B6D00. */

struct CCamera;

/* A camera mode (CCamera.m_Mode): CCamera__SetMode runs enter, CCamera__Advance update. */
typedef struct CameraMode {
    /* 0x0 */ void (*enter)(void);
    /* 0x4 */ void (*update)(struct CCamera *cam);
} CameraMode; /* size 0x8 */
extern CameraMode gCameraModeTable[1]; /* the ROM has the one mode, 0 */

/* The camera player's AI flags, kept three frames (CCamera__DrawTints). */
extern u32 sTintFlags;
extern u32 sTintFlagsPrev;
extern u32 sTintFlagsPrev2;

/* The split-screen views' margins (CCameraPool__Advance). */
extern f32 gSplitScreenMarginBottom;
extern f32 gSplitScreenMarginTop;
extern f32 gSplitScreenMarginLeft;
extern f32 gSplitScreenMarginRight;

extern u32 gDefaultVisBits; /* region visibility bits of things outside any region (-1) */

#endif
