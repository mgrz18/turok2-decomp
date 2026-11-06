#include "common.h"
#include "m2c_macros.h"

s32 CAI__SetMode(s32, s32, s32);

extern s32 D_8012F5F0;

void MantidQueen_UpdateMode2(s32 arg0, s32 arg1) {
    if (D_8012F5F0 == 0) {
        CAI__SetMode(arg0, arg1, 3);
    }
}
