#include "common.h"
#include "m2c_macros.h"

extern s32 func_00201848(s32, s32, s32);

s32 func_00224F84(void *arg0, s32 arg1) {
    s32 *table;

    table = M2C_FIELD(arg0, s32 **, 0x40);
    if (arg1 < *table) {
        return func_00201848((s32)table, M2C_FIELD(arg0, s32 *, 0x14), arg1);
    }
    return 0;
}
