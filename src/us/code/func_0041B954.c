/* WRONG CUT: real function 0x0041B954, size 0x6C: splat cut it into CheatMenu_BlackoutText and func_0041B9B0 at branch targets. */
#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sCheatMenuText_BlackoutOn[];
extern char sCheatMenuText_BlackoutOff[];

/* On/off option item for bit 0x4000: shown only when unlocked. */
s32 CheatMenu_BlackoutText(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x4000) {
        a->flags |= 0x1000000;
    } else {
        a->flags &= ~0x1000000;
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x4000) {
        a->text = sCheatMenuText_BlackoutOn;
    } else {
        a->text = sCheatMenuText_BlackoutOff;
    }
    return 0;
}
