#include "common.h"

void func_00423D30();                            /* extern */
extern s32 D_800C2038;

void Video_SetResolution_2(s32 arg0) {
    D_800C2038 = arg0;
    func_00423D30();
}
