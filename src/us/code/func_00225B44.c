#include "common.h"
#include "m2c_macros.h"

void func_00225B44(void *arg0, s32 arg1) {
    s32 i;
    s32 n;
    void **p;

    i = 0;
    n = M2C_FIELD(arg0, s32 *, 0xDF8);
    p = (void **)((s8 *)arg0 + 0xBF8);
    if (n > 0) {
        do {
            if (M2C_FIELD(*p, s32 *, 0x1A8) == arg1) {
                M2C_FIELD(*p, s32 *, 0x1A8) = 0;
            }
            i++;
            p++;
        } while (i < n);
    }
}
