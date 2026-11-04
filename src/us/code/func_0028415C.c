#include "common.h"
#include "structs/cinema.h"

extern Cinema *D_800C1BB0;

s32 func_0028415C(void) {
    return ~D_800C1BB0->entryIndex != 0;
}
