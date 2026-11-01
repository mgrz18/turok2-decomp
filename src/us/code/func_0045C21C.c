/* Second-module copy of func_004232FC (first module; this one at link address 0x0042321C), calling this module's own functions at their link addresses. */
#include "common.h"
#include "m2c_macros.h"

s32 func_00253640(s32, s32, s32);

s32 func_0045C21C(s32 arg0, void *arg1) {
    void *temp_a0;
    void *temp_a0_2;
    void *temp_v0;

    temp_a0 = M2C_FIELD(arg1, void **, 0x1C);
    if (func_00253640((s32) temp_a0, 0, M2C_FIELD(temp_a0, s32 *, 0x92C)) != 0) {
        temp_v0 = M2C_FIELD(arg1, void **, 0xC);
        M2C_FIELD(temp_v0, s32 *, 0x58) = (s32) (M2C_FIELD(temp_v0, s32 *, 0x58) | 0x01000000);
    } else {
        temp_a0_2 = M2C_FIELD(arg1, void **, 0xC);
        M2C_FIELD(temp_a0_2, s32 *, 0x58) = (s32) (M2C_FIELD(temp_a0_2, s32 *, 0x58) & 0xFEFFFFFF);
    }
    return 0;
}
