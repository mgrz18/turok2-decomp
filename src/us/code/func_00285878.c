#include "common.h"
#include "m2c_macros.h"

s32 osRecvMesg(M2C_UNK *, M2C_UNK, s32);         /* extern */
extern M2C_UNK D_800F6758;

s32 func_00285878(s32 arg0) {
    return osRecvMesg(&D_800F6758, 0, arg0) == 0;
}
