#include "common.h"

/* Pool of 0x1328-byte entries (4904 bytes each) starting at base. */
typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ u32 base;
} Pool0025340C;

u32 func_0025340C(Pool0025340C *arg0, u32 arg1) {
    return (arg1 - arg0->base) / 4904;
}
