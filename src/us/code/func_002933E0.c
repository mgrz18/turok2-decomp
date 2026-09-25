#include "common.h"
#include "m2c_macros.h"

/* Insert at the head of a doubly linked list: next at 0, prev at 4. */
void func_002933E0(void *arg0, void **arg1) {
    M2C_FIELD(arg0, void **, 0) = *arg1;
    M2C_FIELD(arg0, void ***, 4) = arg1;
    if (*arg1 != NULL) {
        M2C_FIELD(*arg1, void **, 4) = arg0;
    }
    *arg1 = arg0;
}
