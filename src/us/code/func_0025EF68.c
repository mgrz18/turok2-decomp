#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

M2C_UNK func_00264C8C(void *, void *, s32);         /* extern */
extern void *D_800F1CE0;

void func_0025EF68(s32 arg0, void *arg1) {
    void *owner;

    if ((M2C_FIELD(arg1, s32 *, 4) != 0) && (D_800F1CE0 != NULL) && (owner = M2C_FIELD(arg1, void **, 0x88), !(M2C_FIELD(arg1, s32 *, 0x3C) & 0x1000)) && (D_800F1CE0 == owner) && (*M2C_FIELD(D_800F1CE0, s32 **, 0x14) == 5)) {
        M2C_FIELD(arg1, void **, 0x114) = (void *) D_800F1CE0;
        func_00264C8C(D_800F1CE0, D_800F1CE0 + 0x140, arg0);
        func_00243414(arg0, (s32) arg1, 0x15);
    }
}
