#include "common.h"
#include "structs/cinema.h"

f32 func_00410BB4(f32);
extern Cinema *D_800C1BB0;

f32 func_00284390(void) {
    f32 value;

    if (D_800C1BB0->active != 0) {
        value = func_00410BB4(D_800C1BB0->time);
    } else {
        value = 47.5f;
    }
    return value;
}
