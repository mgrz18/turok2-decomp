#include "common.h"
#include "structs/cinema.h"

s32 func_00412438();                                /* extern */
extern Cinema *D_800C1BB0;

s32 func_00283FF8(s32 arg0, void (*arg1)(Cinema *), void (*arg2)()) {
    s32 temp_s3;

    temp_s3 = func_00412438();
    if (D_800C1BB0->scriptId == arg0) {
        D_800C1BB0->nextOnClose = arg1;
        D_800C1BB0->nextOnEnd = arg2;
    } else {
        if (arg1 != NULL) {
            arg1(D_800C1BB0);
        }
        if (arg2 != NULL) {
            arg2();
        }
    }
    return temp_s3;
}
