#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 flags;
} Entry;

Entry *func_0022425C(s32 *, s32);                /* extern */
extern s32 D_800F7078;

void func_0026C804(s32 arg0) {
    Entry *temp_v0;

    temp_v0 = func_0022425C(&D_800F7078, arg0);
    if (temp_v0 != NULL) {
        temp_v0->flags = (s32) (temp_v0->flags | 0x40000);
    }
}
