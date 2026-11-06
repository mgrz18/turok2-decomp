#ifndef GLOBALS_BOSS_H
#define GLOBALS_BOSS_H

/* The boss fights' states (structs/boss.h). */

#include "structs/boss.h"

extern WaveBossState gMantidQueen;          /* the Mantid Queen */
extern MotherBossState gMotherBoss;         /* the Mother */
extern BlindOneState gBlindOne;             /* the Blind One */

/* A few files read only the stage, as a plain word: the ROM reloads its
 * address before every read there, which the struct does not give (gcc
 * keeps &gLevelScript in a register). They define BOSS_STAGE_AS_WORD and
 * declare it as an s32 themselves (Primagen_RunAttackStep, Primagen_RunAttackStep_2,
 * Primagen_UpdateMode19, Primagen_UpdateMode19_2). */
#ifndef BOSS_STAGE_AS_WORD
extern LevelScriptState gLevelScript;       /* the level script (the Primagen fight) */
#endif

/* Fields with symbols of their own (kept where the ROM bases the access on them). */
extern WaveBossStep *gMantidQueen_step;     /* gMantidQueen.step */
extern BossState gLevelScript_bossState;    /* gLevelScript from .bossState on */
extern BossMode gLevelScript_bossMode;      /* gLevelScript from .bossMode on */
extern BossScript3 gLevelScript_script3;    /* gLevelScript.script3 */

#endif
