#include "common.h"
#include "m2c_macros.h"

M2C_UNK CCache__Deallocate(M2C_UNK, sptr);                /* extern */
M2C_UNK CCache__Flush(M2C_UNK);                     /* extern */
M2C_UNK Controller_SetRumbleOptionAll(M2C_UNK);                     /* extern */

void CPlayerPool__FreePlayers(void *arg0) {
    s32 temp_a1;

    Controller_SetRumbleOptionAll(0);
    CCache__Flush(0);
    temp_a1 = M2C_FIELD(arg0, s32 *, 0);
    if (temp_a1 != 0) {
        CCache__Deallocate(0, temp_a1);
        M2C_FIELD(arg0, s32 *, 0) = 0;
        M2C_FIELD(arg0, s32 *, 4) = 0;
        M2C_FIELD(arg0, s32 *, 8) = 0;
    }
}
