#ifndef GLOBALS_HUD_H
#define GLOBALS_HUD_H

#include "common.h"
#include "gfx.h"

/* COnScreen (HUD text) */
extern u8 *sOnScreenFontTexture;          /* NULL until COnScreen__LoadFontTexture */
extern u8 sCharacterLookUpPairs[0xB4];    /* {char, glyph} pairs ending 0xFF */
extern u8 sCharacterSmallLookUpPairs[0xB8];
extern s32 sBossBarOverlayScript[5];      /* COverlay scripts */
extern s32 sBossBar2OverlayScript[11];
extern Gfx sHudGlyph16x16LoadDL[8];

/* "<use this to fade out cinema text>": the CCameraPool text fade-out marker */
extern char sCinemaTextFadeOut[0x24];

#endif
