#include "common.h"
#include "structs/cinema.h"

s32 func_00284144();                                /* extern */
s32 func_00284174();                                /* extern */
extern Cinema *D_800C1BB0;

void func_002840EC(void) {
    if ((func_00284174() != 0) && (func_00284144() != 0) && (D_800C1BB0->fading == 0)) {
        D_800C1BB0->fading = 1;
        D_800C1BB0->fade = 1.0f;
    }
}
