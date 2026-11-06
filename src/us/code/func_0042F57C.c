#include "common.h"
#include "structs/demo.h"
#include "m2c_macros.h"


void Demo_Enable(void) {
    D_800F56B0.active = 1;
}
