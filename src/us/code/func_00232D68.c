#include "common.h"
#include "m2c_macros.h"

extern s32 func_00200518(s32, s32);
extern s32 func_00200574(s32, s32);
extern s32 func_002005D0(s32, s32, s32);

/* Insert arg1 into the list at 0xEC14: after the node with the same key at
 * 0xF8 if there is one, else at the end or the front by a flag. */
void func_00232D68(void *arg0, void *arg1) {
    void *p;

    for (p = M2C_FIELD(arg0, void **, 0xEC14); p != NULL; p = M2C_FIELD(p, void **, 0x1D0)) {
        if (M2C_FIELD(p, s32 *, 0xF8) == M2C_FIELD(arg1, s32 *, 0xF8)) {
            break;
        }
    }
    if (p != NULL) {
        func_002005D0((s32)((s8 *)arg0 + 0xEC14), (s32)p, (s32)arg1);
    } else if (*M2C_FIELD(arg1, s32 **, 0xF8) & 0x2000) {
        func_00200518((s32)((s8 *)arg0 + 0xEC14), (s32)arg1);
    } else {
        func_00200574((s32)((s8 *)arg0 + 0xEC14), (s32)arg1);
    }
    M2C_FIELD(arg1, s32 *, 0x40) |= 0x01000000;
}
