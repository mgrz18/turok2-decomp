#include "common.h"
#include "m2c_macros.h"

extern s32 func_00200518(s32, s32);
extern s32 func_00200738(s32, s32);
extern s32 D_8011001C;

s32 func_002283FC(void *arg0, s32 *arg1) {
    s32 node;

    if (D_8011001C != 0 || M2C_FIELD(arg0, u32 *, 0x5324) < 3) {
        node = M2C_FIELD(arg0, s32 *, 0x5300);
        if (node != 0) {
            func_00200738((s32)((s8 *)arg0 + 0x5300), node);
            func_00200518((s32)((s8 *)arg0 + 0x5314), node);
            M2C_FIELD(node, s32 **, 0x290) = arg1;
            if (arg1 != NULL) {
                *arg1 += 1;
            }
        }
        return node;
    }
    return 0;
}
