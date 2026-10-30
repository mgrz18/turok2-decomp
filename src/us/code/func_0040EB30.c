#include "common.h"

extern u8 D_80119870[];
extern u8 D_800F7078[];
extern void func_002535E4(void *players, f32 value);
extern void func_00225D0C(void *);

void func_0040EB30(void) {
    func_002535E4(D_80119870, 900.0f);
    func_00225D0C(D_800F7078);
}
