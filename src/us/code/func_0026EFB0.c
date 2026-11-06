/* Cut fixed in versions/cuts.us.txt: 0x26EFAC is one junk word (`move $v1, $v0`, alignment padding after
 * RumbleCurve_GetValue's object); the real function is an empty one at 0x26EFB0
 * (0x8 bytes: jr $ra; nop), the start of Level_GetHub's object. Nothing
 * calls it or points at it. */
#include "common.h"

void func_0026EFB0(void) {
}
