#include "common.h"

s32 CList__AddHead(s32, s32);
s32 CList__Remove(s32, s32);
s32 __osDisableInt(void);
void __osRestoreInt(s32);

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 inUse;
} CLoaderEntry;

void CLoader__DeallocLoaderEntry(s32 arg0, s32 arg1) {
    s32 temp_s2;

    temp_s2 = __osDisableInt();
    CList__Remove(arg0 + 0x507C, arg1);
    ((CLoaderEntry *)arg1)->inUse = 0;
    CList__AddHead(arg0 + 0x5068, arg1);
    __osRestoreInt(temp_s2);
}
