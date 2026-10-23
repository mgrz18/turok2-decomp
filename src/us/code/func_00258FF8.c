#include "common.h"

void func_00210894(void *mtx, f32 angle);

void func_00258FF8(void *mtx, void *arg1) {
    f32 angle = *(f32 *) (*(s8 **) arg1 + 0x24C) * 1.5f;

    if (*(s32 *) ((s8 *) arg1 + 0x90) == 2) {
        func_00210894(mtx, -angle);
    }
}
