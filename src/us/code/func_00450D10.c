#include "common.h"
#include "structs/controller.h"

/* The game options seen from one player: each player has a 0x40-byte block, so the
 * view moves 0x40 bytes per player and its ready flag sits at 0xA8. */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ u8 ready;
} PlayerOptionsView;

extern s32 gEngineApp_m_Settings;

/* Number of active players, or 0 if any of them is not ready. */
s32 Mp_CountJoinedControllers_2(void) {
    PlayerOptionsView *var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_v1;

    var_a2 = 0;
    var_a1 = 0;
    var_a0 = (PlayerOptionsView *) &gEngineApp_m_Settings;
    var_v1 = 0;
loop_1:
    if (((ControllerProfile *) ((u8 *) gControllers + var_v1))->connected != 0) {
        var_a2 += 1;
        if (var_a0->ready == 0) {
            return 0;
        }
    }
    var_a0 = (PlayerOptionsView *) ((u8 *) var_a0 + 0x40);
    var_a1 += 1;
    var_v1 += sizeof(ControllerProfile);
    if (var_a1 >= 4) {
        return var_a2;
    }
    goto loop_1;
}
