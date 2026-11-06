#ifndef STRUCTS_WARP_FADE_H
#define STRUCTS_WARP_FADE_H

#include "common.h"

/* A row of gWarpFadeColors, by CScene m_WarpFade (CScene__UpdateWarp, CScene__DrawWarpFade). */
typedef struct CWarpFadeColor {
    /* 0x00 */ f32 length;                  /* the fade's length (0: no fade) */
    /* 0x04 */ u8 r;
    /* 0x05 */ u8 g;
    /* 0x06 */ u8 b;
    /* 0x07 */ u8 pad7;
} CWarpFadeColor; /* size 0x8 */

#endif
