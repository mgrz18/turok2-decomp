#include "common.h"
#include "structs/controller.h"

void func_0028582C(ControllerProfile *ctl);

extern u8 D_800B6CF0;
extern ControllerProfile D_800F5C78; /* the four merged */

void func_002857C8(void) {
    ControllerProfile *var_s0;
    s32 var_s1;

    if (D_800B6CF0 != 0) {
        var_s1 = 0;
        var_s0 = D_800F5EC0;
        do {
            func_0028582C(var_s0);
            var_s1 += 1;
            var_s0++;
        } while (var_s1 < 4);
        func_0028582C(&D_800F5C78);
    }
}
