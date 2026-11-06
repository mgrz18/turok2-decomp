#include "common.h"
#include "m2c_macros.h"

/* Length including the terminator. */
s32 HostIo_StrSize(u8 *s) {
    u8 *p = s;

    while (*p++ != 0) {
    }
    return p - s;
}
