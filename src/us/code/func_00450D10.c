#include "common.h"

/* A player slot of the setup screen (0x224 bytes). */
typedef struct {
    /* 0x000 */ s32 active;
    /* 0x004 */ u8 pad4[0x224 - 0x4];
} SetupSlot;

/* The game options seen from one player: each player has a 0x40-byte block, so the
 * view moves 0x40 bytes per player and its ready flag sits at 0xA8. */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ u8 ready;
} PlayerOptionsView;

extern SetupSlot D_800F5EC0[4];
extern s32 D_8011AAD8;

/* Number of active players, or 0 if any of them is not ready. */
s32 func_00450D10(void) {
    PlayerOptionsView *var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_v1;

    var_a2 = 0;
    var_a1 = 0;
    var_a0 = (PlayerOptionsView *) &D_8011AAD8;
    var_v1 = 0;
loop_1:
    if (((SetupSlot *) ((u8 *) D_800F5EC0 + var_v1))->active != 0) {
        var_a2 += 1;
        if (var_a0->ready == 0) {
            return 0;
        }
    }
    var_a0 = (PlayerOptionsView *) ((u8 *) var_a0 + 0x40);
    var_a1 += 1;
    var_v1 += sizeof(SetupSlot);
    if (var_a1 >= 4) {
        return var_a2;
    }
    goto loop_1;
}
