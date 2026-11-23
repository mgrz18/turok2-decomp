#include "common.h"

#ifdef TARGET_PORT
f32 Random_Float(f32 scale); /* prototyped: an unprototyped call would pass the f32 as a double */
#else
s32 Random_Float();                            /* extern */
#endif

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS Random_Float receives func_00232E80's own argument
 * registers (pass-through); the host passes them. */
void func_00232E80(f32 arg0) {
    if (arg0 != 0.0f) {
        Random_Float(arg0);
    }
}
#else
void func_00232E80(f32 arg0) {
    if (arg0 != 0.0f) {
        Random_Float();
    }
}
#endif
