#include "common.h"
#include "m2c_macros.h"

s32 CList__AddHead(s32, s32);
s32 CList__Remove(s32, s32);
s32 __osDisableInt(void);
void __osRestoreInt(s32);

void CLoader__DeallocLoaderEntry(s32 arg0, s32 arg1) {
    s32 temp_s2;

    temp_s2 = __osDisableInt();
    CList__Remove(arg0 + 0x507C, arg1);
    M2C_FIELD(arg1, s32 *, 0x14) = 0;
    CList__AddHead(arg0 + 0x5068, arg1);
    __osRestoreInt(temp_s2);
}
