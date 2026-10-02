#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0042444C();                            /* extern */
extern f32 D_800C132C;
extern u8 D_800C203B;
extern u8 D_8011AC58;
extern f32 D_80130930;

s32 func_00455788(void) {
    if (D_80130930 == 0.0f) {
        func_0042444C();
        D_80130930 = D_800C132C;
        D_8011AC58 = D_800C203B;
    }
    return 0;
}
