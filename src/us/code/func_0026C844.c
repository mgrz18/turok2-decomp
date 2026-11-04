#include "common.h"

/* The region info func_0022425C returns (as in func_0023B664.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 flags;
} RegionInfo;

RegionInfo *func_0022425C(void *, s32);             /* extern */
extern u8 D_800F7078[];

void func_0026C844(s32 arg0) {
    RegionInfo *temp_v0;

    temp_v0 = func_0022425C(D_800F7078, arg0);
    if (temp_v0 != NULL) {
        temp_v0->flags = (s32) (temp_v0->flags & 0xFFFBFFFF);
    }
}
