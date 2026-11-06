#include "common.h"
#include "structs/cinema.h"

extern Cinema *D_800C1BB0;

s32 Cinema_IsActive(void) {
    return D_800C1BB0->active;
}
