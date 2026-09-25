#include "common.h"
#include "m2c_macros.h"

s32 func_002017D4(s32, s32);

s32 func_002242F8(void *arg0, s32 arg1) {
    { s32 base = func_002017D4(M2C_FIELD(arg0, s32 *, 0x58), 2) + 8; return base + (arg1 << 5); }
}
