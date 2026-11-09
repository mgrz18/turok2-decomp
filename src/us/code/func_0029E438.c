#include "common.h"
#include "m2c_macros.h"

sptr func_0029E460(sptr, sptr, s32);                /* memcpy */

/* sprintf's output routine: copies count bytes to dst and returns the end. m2c dropped the copy's
 * arguments (still in a0-a2 on the N64). */
sptr proutSprintf(sptr arg0, sptr arg1, s32 arg2) {
    return func_0029E460(arg0, arg1, arg2) + arg2;
}
