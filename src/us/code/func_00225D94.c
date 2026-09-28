#include "common.h"
#include "m2c_macros.h"

/* Element i of a table whose first word is the element size, data at +8. */
void *func_00225D94(void *arg0, s32 arg1) {
    s32 *t = M2C_FIELD(arg0, s32 **, 0x84);

    return (s8 *)t + (arg1 * *t + 8);
}
