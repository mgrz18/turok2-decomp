#include "common.h"
#include "m2c_macros.h"

s32 CList__AddHead(s32, s32);
s32 CList__Remove(s32, s32);

void func_0042FFB0(s32 arg0, s32 arg1) {
    CList__Remove(arg0 + 0x20, arg1);
    CList__AddHead(arg0 + 0xC, arg1);
}
