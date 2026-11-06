#ifndef GLOBALS_CAMERA_H
#define GLOBALS_CAMERA_H

/* The camera pool of the game, gEngineApp_m_Game.m_CameraPool (CGame + 0x40). */

#include "structs/camera.h"
#include "structs/collision.h"

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

#endif
