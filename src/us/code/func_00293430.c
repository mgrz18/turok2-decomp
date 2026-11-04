#include "common.h"

void func_00294150();                            /* extern */
extern s32 D_800B7760[];

void func_00293430(void) {
    if (D_800B7760[0] != 0) {
        func_00294150();
        D_800B7760[0] = 0;
    }
}
