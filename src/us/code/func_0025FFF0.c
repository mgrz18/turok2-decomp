#include "common.h"

s32 func_00243414(s32, s32, s32);

/* Three override slots, 6 bytes apart (see func_0025AC88). */
extern u16 D_800F5C66[];

void func_0025FFF0(s32 arg0, s32 arg1) {
    if ((D_800F5C66[0] != 0) || (D_800F5C66[3] != 0) || (D_800F5C66[6] != 0)) {
        func_00243414(arg0, arg1, 0x32);
    }
}
