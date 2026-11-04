#include "common.h"
#include "structs/player.h"

/* Whether the player is in state 3 with 0xC04 in 0x7DF..0x7E3. */
s32 func_00256D44(CPlayer *arg0) {
    s32 temp_a0;
    s32 var_a1;

    var_a1 = 0;
    if (arg0->m_Mode == 3) {
        temp_a0 = arg0->m_CharAnim;
        if (temp_a0 >= 0x7DF) {
            var_a1 = temp_a0 < 0x7E4;
        }
    }
    return var_a1;
}
