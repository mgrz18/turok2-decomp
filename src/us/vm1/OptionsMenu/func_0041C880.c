/* WRONG CUT: real function 0x0041C880, size 0x4C (through 0x0041C8CC): splat cut it in two
 * (OptionsMenu_StepBrightness, func_0041C89C) in the middle of a call's argument setup. */
#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/menu.h"


s32 OptionsMenu_StepBrightness(s32 menu, void *c) {
    GameSettings *s = &gEngineApp_m_Settings;

    s->unkC = Menu_StepValue(c, s->unkC, 0x10, 0, 0x80, 0);
    return 0;
}
