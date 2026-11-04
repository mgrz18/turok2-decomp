#include "common.h"

extern s32 D_800F5EC0;          /* controller states, 0x224 bytes each; first word: connected */
extern s32 D_8011AAD8;          /* settings; per-player 0x40-byte blocks, flag at +0xA8 */

/* Counts the connected controllers; 0 if one of their players is not ready. */
s32 func_00417D10(void) {
    u8 *var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_v1;

    var_a2 = 0;
    var_a1 = 0;
    var_a0 = (u8 *) &D_8011AAD8;
    var_v1 = 0;
loop_1:
    if (*(s32 *) ((u8 *) &D_800F5EC0 + var_v1) != 0) {
        var_a2 += 1;
        if (var_a0[0xA8] == 0) {
            return 0;
        }
    }
    var_a0 = var_a0 + 0x40;
    var_a1 += 1;
    var_v1 += 0x224;
    if (var_a1 >= 4) {
        return var_a2;
    }
    goto loop_1;
}
