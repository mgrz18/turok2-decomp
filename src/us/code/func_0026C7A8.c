#include "common.h"

/* A 0x64-byte record of the world model sections (func_0022425C finds it from an id). */
typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 flags;
    /* 0x48 */ u8 pad48[0x64 - 0x48];
} ModelRecord; /* size 0x64 */

ModelRecord *func_0022425C(s32 *, u16 *);              /* extern */
s32 func_0026C9B8(u16 *, u16);                  /* extern */
extern s32 D_800F7078;

void func_0026C7A8(u16 *arg0) {
    ModelRecord *temp_v0;

    temp_v0 = func_0022425C(&D_800F7078, arg0);
    if (temp_v0 != NULL) {
        temp_v0->flags |= 0x4000;
    }
    if (arg0 != NULL) {
        func_0026C9B8(arg0, *arg0);
    }
}
