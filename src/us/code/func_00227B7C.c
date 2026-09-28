#include "common.h"
#include "m2c_macros.h"

void func_00227B7C(void *arg0, void *arg1) {
    s32 *temp_v1;
    s32 temp_a2;
    void *temp_v1_2;
    void *temp_v1_3;

    temp_a2 = M2C_FIELD(arg1, s32 *, 0x1A0);
    if (temp_a2 & 1) {
        temp_v1 = M2C_FIELD(arg1, s32 **, 0x1A4);
        M2C_FIELD(arg1, s32 *, 0x1A0) = (s32) (temp_a2 & ~1);
        if (temp_v1 != NULL) {
            *temp_v1 -= 1;
        }
        temp_v1_2 = M2C_FIELD(arg1, void **, 0x1A8);
        if (temp_v1_2 != NULL) {
            M2C_FIELD(temp_v1_2, void **, 0x1AC) = (void *) M2C_FIELD(arg1, void **, 0x1AC);
        }
        temp_v1_3 = M2C_FIELD(arg1, void **, 0x1AC);
        if (temp_v1_3 != NULL) {
            M2C_FIELD(temp_v1_3, void **, 0x1A8) = (void *) M2C_FIELD(arg1, void **, 0x1A8);
        }
        if (M2C_FIELD(arg0, void **, 0x3604) == arg1) {
            M2C_FIELD(arg0, void **, 0x3604) = (void *) M2C_FIELD(arg1, void **, 0x1AC);
        }
        if (M2C_FIELD(arg0, void **, 0x3608) == arg1) {
            M2C_FIELD(arg0, void **, 0x3608) = (void *) M2C_FIELD(arg1, void **, 0x1A8);
        }
        M2C_FIELD(arg1, void **, 0x1AC) = (void *) M2C_FIELD(arg0, void **, 0x3600);
        M2C_FIELD(arg1, void **, 0x1A8) = NULL;
        M2C_FIELD(arg0, void **, 0x3600) = arg1;
    }
}
