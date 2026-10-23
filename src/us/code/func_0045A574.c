#include "common.h"
#include "m2c_macros.h"
#include "rw/abu_protos.h"

extern M2C_UNK D_0043BDF0;
extern M2C_UNK D_0043BE14;
extern s32 D_80130940;

s32 func_0045A574(void *arg0) {
    s32 temp_v1;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0x24);
    if ((temp_v1 == &D_0043BE14) || (temp_v1 == &D_0043BDF0)) {
        D_80130940 = 0;
        return 1;
    }
    return 0;
}
