#include "common.h"

void func_00275A74();                            /* extern */
void func_00275ABC();                            /* extern */
void func_00275E1C(s32);                         /* extern */
s32 func_00275F7C();                                /* extern */

s32 func_00419A8C(void) {
    func_00275E1C(func_00275F7C());
    func_00275A74();
    func_00275ABC();
    return 1;
}
