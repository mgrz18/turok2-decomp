#include "common.h"

/* A record that owns a handle at 0x14 (passed to func_00290EF0) and an index at 0x28. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 handle;
    /* 0x18 */ u8 pad18[0x28 - 0x18];
    /* 0x28 */ s32 index;
} HandleOwner;

s32 func_00290EF0(s32);                         /* extern */

void func_00275248(HandleOwner *arg0) {
    arg0->index = -1;
    func_00290EF0(arg0->handle);
}
