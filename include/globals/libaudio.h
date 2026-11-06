#ifndef GLOBALS_LIBAUDIO_H
#define GLOBALS_LIBAUDIO_H

/* libaudio's data, 0x800B7760-0x800B7A1F (ultralib's names). */

#include "common.h"
#include "structs/libaudio.h" /* ALGlobals */

extern ALGlobals *alGlobals;

/* alFxNew's reverb presets (drvrnew.c): section count, length, then 8 words per section. */
extern s32 SMALLROOM_PARAMS[26];
extern s32 BIGROOM_PARAMS[34];
extern s32 ECHO_PARAMS[10];
extern s32 CHORUS_PARAMS[10];
extern s32 FLANGE_PARAMS[10];
extern s32 NULL_PARAMS[10];

extern s16 eqpower[128]; /* the equal-power pan curve (env.c) */

#endif
