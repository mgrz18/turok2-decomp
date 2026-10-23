#include "common.h"
#include "m2c_macros.h"
#include "rw/abu_protos.h"

extern s32 D_8011ACE4;

void func_0041CD7C(void *arg0) {
    void *temp_a0;
    void *temp_v0;

    if (D_8011ACE4 != 0) {
        temp_v0 = M2C_FIELD(arg0, void **, 0xC);
        M2C_FIELD(temp_v0, s32 *, 0x120) = (s32) (M2C_FIELD(temp_v0, s32 *, 0x120) | 0x01000000);
        return;
    }
    temp_a0 = M2C_FIELD(arg0, void **, 0xC);
    M2C_FIELD(temp_a0, s32 *, 0x120) = (s32) (M2C_FIELD(temp_a0, s32 *, 0x120) & 0xFEFFFFFF);
}
