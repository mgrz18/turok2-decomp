#include "common.h"
#include "structs/controller.h"

extern s32 gEngineApp_m_Settings;          /* settings; per-player 0x40-byte blocks, flag at +0xA8 */

/* Counts the connected controllers; 0 if one of their players is not ready. */
s32 Mp_CountJoinedControllers(void) {
    u8 *var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_v1;

    var_a2 = 0;
    var_a1 = 0;
    var_a0 = (u8 *) &gEngineApp_m_Settings;
    var_v1 = 0;
loop_1:
    if (((ControllerProfile *) ((u8 *) gControllers + var_v1))->connected != 0) {
        var_a2 += 1;
        if (var_a0[0xA8] == 0) {
            return 0;
        }
    }
    var_a0 = var_a0 + 0x40;
    var_a1 += 1;
    var_v1 += sizeof(ControllerProfile);
    if (var_a1 >= 4) {
        return var_a2;
    }
    goto loop_1;
}
