#include "common.h"
#include "m2c_macros.h"

s32 func_002759C4(s32);
s32 func_00284080(void);
s32 func_00285410(s32);
s32 func_0042F514(void);

M2C_UNK func_00275904(M2C_UNK);                     /* extern */
M2C_UNK func_00275A08();                            /* extern */
extern M2C_UNK D_800B6D78;
extern s32 D_8011B110;

void func_00289DFC(void *arg0) {
    M2C_UNK (*temp_v0)(void *);
    s32 temp_v1;

    if ((D_8011B110 == 2) && (M2C_FIELD(arg0, s32 *, 0x23FF0) == 0)) {
        M2C_FIELD(arg0, s32 *, 0x23FD0) = 0;
        M2C_FIELD(arg0, s32 *, 0x23FD8) = (s32) M2C_FIELD(arg0, s32 *, 0x23FDC);
        func_00284080();
        func_00275904(-1);
        temp_v1 = M2C_FIELD(arg0, s32 *, 0x23FDC);
        if ((temp_v1 != 8) && (temp_v1 != 0xC)) {
            if ((M2C_FIELD(arg0, s32 *, 0x23FD4) != 8) || (M2C_FIELD(arg0, s32 *, 0x23FD8) != 0xD)) {
                func_002759C4(-1);
                func_00275A08();
                goto block_7;
            }
        } else {
block_7:
            if (M2C_FIELD(arg0, s32 *, 0x23FD8) != 0xD) {
                func_0042F514();
            }
        }
        func_00285410(0);
        temp_v0 = *(((__typeof__(&D_800B6D78))((s8 *)&D_800B6D78 + (M2C_FIELD(arg0, s32 *, 0x23FD8) * 0xC))));
        if (temp_v0 != NULL) {
            temp_v0(arg0);
        }
    }
}
