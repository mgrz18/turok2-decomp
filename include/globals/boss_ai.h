#ifndef GLOBALS_BOSS_AI_H
#define GLOBALS_BOSS_AI_H

/* The bosses' AI mode tables (CAIModeInfo rows, set as m_pModeTable), in the
 * first VM module's data, and the module-2 copies (`_2`). Apart from
 * globals/boss.h because they need structs/game_object.h. */

#include "structs/game_object.h"

extern CAIModeInfo sMantidQueenModeTable[];
extern CAIModeInfo sPrimagenModeTable[];
extern CAIModeInfo sMotherBossModeTable[];

/* Module-2 copies. */
extern CAIModeInfo sPrimagenModeTable_2[];
extern CAIModeInfo sMotherBossModeTable_2[];

/* The Blind One's (module 1 and the module-2 code share them) */
extern CAIModeInfo gBlindOneBossModeTable[];
extern CAIModeInfo gBlindOneMinionModeTable[];

#endif
