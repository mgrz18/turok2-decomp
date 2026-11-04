#include "common.h"
#include "structs/cinema.h"

extern Cinema *D_800C1BB0;

s32 func_00284144(void) {
    return D_800C1BB0->scriptId == 0x50;
}
