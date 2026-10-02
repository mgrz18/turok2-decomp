#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

extern s32 D_8012F5F0;

void func_00402E44(s32 arg0, s32 arg1) {
    if (D_8012F5F0 == 0) {
        func_00243414(arg0, arg1, 6);
    }
}
