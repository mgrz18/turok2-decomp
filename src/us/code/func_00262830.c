#include "common.h"

s32 func_00243414(s32, s32, s32);       /* extern */
s32 func_002623EC();                                /* extern */

void func_00262830(s32 arg0, s32 arg1) {
    if (func_002623EC() != 0) {
        func_00243414(arg0, arg1, 0x3F);
    }
}
