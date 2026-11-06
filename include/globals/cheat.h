#ifndef GLOBALS_CHEAT_H
#define GLOBALS_CHEAT_H

/* The cheat code table of the menu module (EnterCheatMenu_Enter), and its
 * module-2 copy (`_2`). CheatCode {code, unk4, flags, sound} is Rage Wars'
 * (rw/abu_types.h); unk4 is the result text EnterCheatMenu_ResultLabel shows. */

#include "rw/abu_types.h"

extern CheatCode sCheatCodes[12];
extern CheatCode sCheatCodes_2[12];
extern s32 sCheatCodes_unk4_2; /* = &sCheatCodes_2[0].unk4: EnterCheatMenu_ResultLabel_2 indexes it by 0x10 */

#endif
