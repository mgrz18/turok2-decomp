#include "common.h"
#include "m2c_macros.h"

extern s32 func_0020C0D0(s32, s32, s32);

void func_0021E7F8(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, void (**)(void *, void *), 0x244)(arg0, (s8 *)arg0 + 0x140);
    func_0020C0D0((s32)((s8 *)arg0 + 0x58), arg1, (s32)((s8 *)arg0 + 0xBC));
}
