#ifndef GLOBALS_CINEMA_H
#define GLOBALS_CINEMA_H

#include "structs/cinema.h"
#include "common.h"

extern Cinema *gCinemaPlayerP;      /* Turok 3's cinema player */
extern s32 gCinemaEnabled;          /* Cinema_Enable / Cinema_Disable */

extern s32 sCinemaLastFrame; /* gFrameCount when Cinema_Update last ran (-1) */

/* Round 30 s2 (were D_800C1BC4 / D_800C1BC8, the VM data blob) */
extern s32 gCinemaHoldFade;         /* Cinema_SetHoldFade: the cinema draw keeps the fade while it is set */
extern f32 gCinemaFadeBias;         /* Cinema_GetFadeAlpha stretches the fade value by it */

#endif
