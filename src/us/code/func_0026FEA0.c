#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_002933B0(void *);                      /* extern */
M2C_UNK func_002933E0(void *, void *);              /* extern */
M2C_UNK func_0029B6F0(M2C_UNK *, s32 *, M2C_UNK);   /* extern */
extern u32 D_800B2660;
extern u32 D_800B2664;
extern M2C_UNK D_800F2778;
extern M2C_UNK D_800F2794;

void func_0026FEA0(void) {
    s32 sp10;
    u32 var_s0;
    void *temp_s1;
    void *var_s0_2;

    var_s0 = 0;
    sp10 = 0;
    if (D_800B2664 != 0) {
        do {
            func_0029B6F0(&D_800F2778, &sp10, 1);
            var_s0 += 1;
        } while (var_s0 < (u32) D_800B2664);
    }
    var_s0_2 = M2C_FIELD(&D_800F2794, void **, 0);
    if (var_s0_2 != NULL) {
        do {
            temp_s1 = M2C_FIELD(var_s0_2, void **, 0);
            if ((u32) (M2C_FIELD(var_s0_2, s32 *, 0xC) + 1) < (u32) D_800B2660) {
                if (M2C_FIELD(&D_800F2794, void **, 0) == var_s0_2) {
                    M2C_FIELD(&D_800F2794, void **, 0) = temp_s1;
                }
                func_002933B0(var_s0_2);
                if (M2C_FIELD(&D_800F2794, void **, 4) == NULL) {
                    M2C_FIELD(&D_800F2794, void **, 4) = var_s0_2;
                    M2C_FIELD(var_s0_2, void **, 0) = NULL;
                    M2C_FIELD(var_s0_2, s32 *, 4) = 0;
                    goto block_11;
                }
                func_002933E0(var_s0_2, M2C_FIELD(&D_800F2794, void **, 4));
                var_s0_2 = temp_s1;
            } else {
block_11:
                var_s0_2 = temp_s1;
            }
        } while (var_s0_2 != NULL);
    }
    D_800B2664 = 0;
    D_800B2660 += 1;
}
