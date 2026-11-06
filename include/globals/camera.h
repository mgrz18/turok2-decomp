#ifndef GLOBALS_CAMERA_H
#define GLOBALS_CAMERA_H

/* What the active cameras see (CCameraPool__Advance). */

#include "structs/collision.h"

extern CBox gCameraVisibleBox;      /* widened for animation */
extern CBox gCameraViewBox;         /* the plain view boxes */
extern u32 gCameraVisBits;          /* the visibility bits of the active views */
extern f32 gScreenClearTime;        /* seconds the screen is cleared for while the view moves */

#endif
