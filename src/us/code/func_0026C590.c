#include "common.h"
#include "m2c_macros.h"

extern s32 func_0026C934(void *, u16);

void func_0026C590(void *arg0) {
    if ((arg0 != NULL) && (M2C_FIELD(arg0, u16 *, 2) & 2)) {
        func_0026C934(arg0, M2C_FIELD(arg0, u16 *, 0));
    }
}
