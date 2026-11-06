#ifndef GLOBALS_CINEMA_H
#define GLOBALS_CINEMA_H

#include "structs/cinema.h"
#include "common.h"

extern Cinema *gCinemaPlayerP;      /* Turok 3's cinema player */
extern s32 gCinemaEnabled;          /* Cinema_Enable / Cinema_Disable */

extern s32 sCinemaLastFrame; /* gFrameCount when Cinema_Update last ran (-1) */

#endif
