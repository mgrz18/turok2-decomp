#ifndef GLOBALS_BOSS_AI_H
#define GLOBALS_BOSS_AI_H

/* The bosses' AI mode tables (CAIModeInfo rows, set as m_pModeTable), in the
 * first VM module's data, and the module-2 copies (`_2`). Apart from
 * globals/boss.h because they need structs/game_object.h. */

#include "structs/game_object.h"

/* sized (round 10 y2): the header row, modes 0-16 and the -1 row. Unsized, gendata measured it to
 * the next label of the module-2 copy that overlaps the same addresses (0x180 of its 0x260 bytes):
 * the host table stopped after mode 10, so modes 11-16 (the spot-3 opening 14/15, the death 16)
 * were never found and CAI__SetMode left the Queen in her old mode. */
extern CAIModeInfo sMantidQueenModeTable[19];
extern CAIModeInfo sPrimagenModeTable[];
extern CAIModeInfo sMotherBossModeTable[];

/* Module-2 copies. */
extern CAIModeInfo sPrimagenModeTable_2[];
extern CAIModeInfo sMotherBossModeTable_2[];

/* The Blind One's (module 1 and the module-2 code share them) */
extern CAIModeInfo gBlindOneBossModeTable[];
extern CAIModeInfo gBlindOneMinionModeTable[];

#endif
