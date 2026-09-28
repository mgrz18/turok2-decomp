#include "common.h"
#include "m2c_macros.h"

/* strcpy */
u8 *func_00288800(u8 *dst, u8 *src) {
    u8 *d = dst;

    while ((*d++ = *src++) != 0) {
    }
    return dst;
}
