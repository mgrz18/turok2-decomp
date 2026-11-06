#ifndef GLOBALS_CAMERA_H
#define GLOBALS_CAMERA_H

/* The cameras' data, 0x800B6CB0-0x800B6D00. */

#include "common.h"

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
