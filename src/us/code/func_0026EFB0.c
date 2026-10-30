/* Cut fixed in versions/cuts.us.txt: 0x26EFAC is one junk word (`move $v1, $v0`, alignment padding after
 * func_0026EF30's object); the real function is an empty one at 0x26EFB0
 * (0x8 bytes: jr $ra; nop), the start of func_0026EFB8's object. Nothing
 * calls it or points at it. */
#include "common.h"

void func_0026EFB0(void) {
}
