#include "common.h"

s32 CList__AddHead(s32, s32);                /* extern */
s32 CList__Remove(s32);                         /* extern */

void func_00283730(s32 arg0, s32 arg1) {
    CList__Remove(arg0 + 0x8B4);
    CList__AddHead(arg0 + 0x8A0, arg1);
}
