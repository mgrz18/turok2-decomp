#include "common.h"
#include "structs/pager.h"


/* The memory region containing page arg0. */
s32 *func_0028EA40(u32 arg0) {
    PageRegion *var_v1;
    u16 temp_a1;

    var_v1 = &D_80121378;
    if (&D_80121378 != NULL) {
loop_1:
        temp_a1 = var_v1->start;
        if ((arg0 < temp_a1) || (arg0 >= (u32) (temp_a1 + var_v1->pages))) {
            var_v1 = var_v1->next;
            if (var_v1 != NULL) {
                goto loop_1;
            }
        }
    }
    return (s32 *) var_v1;
}
