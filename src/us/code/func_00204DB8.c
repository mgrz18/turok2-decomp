#include "common.h"

typedef struct {
    /* 0x0 */ u8 pad0[8];
    /* 0x8 */ s32 refCount;
    /* 0xC */ s32 flags;
} Arg0;

/* Drops a reference; clears flag 0x100 at zero. */
void CMemEntry__Unlock(Arg0 *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->refCount - 1;
    arg0->refCount = temp_v0;
    if (temp_v0 == 0) {
        arg0->flags = (s32) (arg0->flags & ~0x100);
    }
}
