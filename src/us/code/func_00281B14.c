#include "common.h"

void CList__AddHead(s32, s32);                /* extern */
void CList__Remove(s32, s32);                /* extern */

void func_00281B14(s32 arg0, s32 arg1, s32 arg2) {
    CList__Remove(arg1 + 0xE28, arg2);
    CList__AddHead(arg0 + 0xF0C, arg2);
}
