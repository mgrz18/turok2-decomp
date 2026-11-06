#include "common.h"

s32 memset(s32, s32, s32);           /* extern */
s32 osCreateMesgQueue(s32, s32, s32);           /* extern */
s32 osCreateThread(s32, s32, s32 *, s32, s32, s32); /* extern */
s32 osStartThread(s32);                         /* extern */
extern s32 D_800B6E7C;
extern s32 CDecompressor__Main;

void CDecompressor__Construct(s32 arg0, s32 arg1) {
    osCreateMesgQueue(arg0 + 0x230, arg0 + 0x248, 0x80);
    memset(arg0 + 0x448, arg1 & 0xFF, 0x1000);
    osCreateThread(arg0, arg1, &CDecompressor__Main, arg0, arg0 + 0x1448, D_800B6E7C);
    osStartThread(arg0);
}
