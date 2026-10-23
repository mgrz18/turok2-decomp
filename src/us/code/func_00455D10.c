#include "common.h"
#include "m2c_macros.h"
#include "rw/abu_protos.h"

extern s32 D_8011ACE4;

void func_00455D10(void *arg0) {
    void *temp_a0;
    void *temp_v0;
    void *temp_v1;
    void *temp_v1_2;
    void *temp_v1_3;
    void *temp_v1_4;
    void *temp_v1_5;
    void *temp_v1_6;

    temp_v1 = M2C_FIELD(arg0, void **, 0x1C);
    if (temp_v1 != NULL) {
        M2C_FIELD(temp_v1, s32 *, 0xBF8) = 1;
    }
    temp_v1_2 = M2C_FIELD(arg0, void **, 0xC);
    M2C_FIELD(temp_v1_2, s32 *, 0x1C0) = (s32) (M2C_FIELD(temp_v1_2, s32 *, 0x1C0) | 0x01800000);
    temp_v1_3 = M2C_FIELD(arg0, void **, 0xC);
    M2C_FIELD(temp_v1_3, s32 *, 0x1E8) = (s32) (M2C_FIELD(temp_v1_3, s32 *, 0x1E8) | 0x01800000);
    temp_v1_4 = M2C_FIELD(arg0, void **, 0xC);
    M2C_FIELD(temp_v1_4, s32 *, 0x210) = (s32) (M2C_FIELD(temp_v1_4, s32 *, 0x210) | 0x01800000);
    temp_v1_5 = M2C_FIELD(arg0, void **, 0xC);
    M2C_FIELD(temp_v1_5, s32 *, 0x238) = (s32) (M2C_FIELD(temp_v1_5, s32 *, 0x238) & 0xFE7FFFFF);
    temp_v1_6 = M2C_FIELD(arg0, void **, 0xC);
    M2C_FIELD(temp_v1_6, s32 *, 0x260) = (s32) (M2C_FIELD(temp_v1_6, s32 *, 0x260) & 0xFE7FFFFF);
    if (D_8011ACE4 != 0) {
        temp_v0 = M2C_FIELD(arg0, void **, 0xC);
        M2C_FIELD(temp_v0, s32 *, 0x120) = (s32) (M2C_FIELD(temp_v0, s32 *, 0x120) | 0x01000000);
        return;
    }
    temp_a0 = M2C_FIELD(arg0, void **, 0xC);
    M2C_FIELD(temp_a0, s32 *, 0x120) = (s32) (M2C_FIELD(temp_a0, s32 *, 0x120) & 0xFEFFFFFF);
}
