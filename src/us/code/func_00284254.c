#include "common.h"
#include "structs/cinema.h"

void func_00410CE8();                               /* extern */
extern Cinema *D_800C1BB0;

void func_00284254(void) {
    if (D_800C1BB0->active != 0) {
        func_00410CE8();
    }
}
