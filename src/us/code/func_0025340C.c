#include "common.h"

/* Pool of 0x1328-byte entries (4904 bytes each) starting at base. */
typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ u32 base;
} Pool0025340C;

uptr CPlayerPool__GetIndex(Pool0025340C *arg0, uptr arg1) {
    return (arg1 - arg0->base) / 4904;
}
