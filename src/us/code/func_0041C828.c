#include "common.h"
#include "m2c_macros.h"

s32 func_0042452C(void);

extern f32 D_800C132C;
extern u8 D_800C203B;
extern u8 D_8011AC58;
extern f32 D_80130930;

s32 func_0041C828(void) {
    if (D_80130930 == 0.0f) {
        func_0042452C();
        D_80130930 = D_800C132C;
        D_8011AC58 = D_800C203B;
    }
    return 0;
}
