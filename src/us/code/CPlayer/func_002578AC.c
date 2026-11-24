#include "common.h"
#include "m2c_macros.h"

void CCache__Deallocate(sptr, void *);
void CCache__Flush(s32);
void Controller_SetRumbleOptionAll(s32);

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
