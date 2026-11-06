#ifndef GLOBALS_TEXT_H
#define GLOBALS_TEXT_H

/* The game's text tables in .data, 0x800B66DC-0x800B6CAF. */

#include "common.h"

extern char *gCreditsLines[]; /* the credits, one line each, NULL-terminated (Credits_Draw, Credits_Update) */

/* A level message (LevelEvent_Fire, Action_CheckRequiredItem, ImpactEvent_PlaySoundAndEffect). */
typedef struct LevelMessage {
    /* 0x0 */ char *text;
    /* 0x4 */ s32 speech; /* the speech sound played with it, 0 for none */
} LevelMessage; /* size 0x8 */
extern LevelMessage gLevelMessages[];

#endif
