#include "common.h"
#include "structs/cinema.h"

s32 func_00411EF8();                            /* extern */
extern Cinema *D_800C1BB0;

void func_00284284(void) {
    if (D_800C1BB0->active != 0) {
        func_00411EF8();
    }
}
