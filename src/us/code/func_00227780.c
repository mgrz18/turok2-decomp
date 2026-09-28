#include "common.h"
#include "m2c_macros.h"

extern void *D_8010AB90;
extern void *D_8010AB94;
extern void *D_8010AB98;

void func_00227780(void *arg0) {
    s32 *temp_v1;
    s32 temp_a1;
    void *temp_v1_2;
    void *temp_v1_3;

    temp_a1 = M2C_FIELD(arg0, s32 *, 0x1A0);
    if (temp_a1 & 1) {
        temp_v1 = M2C_FIELD(arg0, s32 **, 0x1A4);
        M2C_FIELD(arg0, s32 *, 0x1A0) = (s32) (temp_a1 & ~1);
        if (temp_v1 != NULL) {
            *temp_v1 -= 1;
        }
        temp_v1_2 = M2C_FIELD(arg0, void **, 0x1A8);
        if (temp_v1_2 != NULL) {
            M2C_FIELD(temp_v1_2, void **, 0x1AC) = (void *) M2C_FIELD(arg0, void **, 0x1AC);
        }
        temp_v1_3 = M2C_FIELD(arg0, void **, 0x1AC);
        if (temp_v1_3 != NULL) {
            M2C_FIELD(temp_v1_3, void **, 0x1A8) = (void *) M2C_FIELD(arg0, void **, 0x1A8);
        }
        if (D_8010AB94 == arg0) {
            D_8010AB94 = M2C_FIELD(arg0, void **, 0x1AC);
        }
        if (D_8010AB98 == arg0) {
            D_8010AB98 = M2C_FIELD(arg0, void **, 0x1A8);
        }
        M2C_FIELD(arg0, void **, 0x1AC) = (void *) D_8010AB90;
        M2C_FIELD(arg0, void **, 0x1A8) = NULL;
        D_8010AB90 = arg0;
    }
}
