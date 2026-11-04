#include "common.h"

s32 func_00275A74();                            /* extern */
s32 func_00275ABC();                            /* extern */
s32 func_00275E1C(s32);                         /* extern */
s32 func_00275F7C();                                /* extern */

s32 func_004529EC(void) {
    func_00275E1C(func_00275F7C());
    func_00275A74();
    func_00275ABC();
    return 1;
}
