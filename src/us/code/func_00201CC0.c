#include "common.h"

s32 CList__Construct(s32, s32, s32);       /* extern */
s32 CList__AddTail(s32, void *);                 /* extern */
s32 memset(s32, s32, s32);           /* extern */
s32 osCreateMesgQueue(s32, s32, s32);           /* extern */
s32 osCreateThread(s32, s32, s32 *, s32, s32, s32); /* extern */
s32 osSetEventMesg(s32, s32, s32);       /* extern */
s32 osStartThread(s32);                         /* extern */
extern s32 D_800B6E94;
extern s32 CLoader__Main;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 inUse;
} CLoaderEntry;

void CLoader__Construct(s32 arg0, s32 arg1) {
    s32 temp_s0;
    s32 var_s0;
    s32 var_s1;
    void *temp_a1;

    osCreateMesgQueue(arg0 + 0x230, arg0 + 0x248, 0x200);
    temp_s0 = arg0 + 0xA48;
    osCreateMesgQueue(temp_s0, arg0 + 0xA60, 1);
    osSetEventMesg(8, temp_s0, 0x7D1);
    CList__Construct(arg0 + 0x5068, 0x18, 0x1C);
    CList__Construct(arg0 + 0x507C, 0x18, 0x1C);
    var_s1 = 0;
    var_s0 = 0xE68;
    do {
        temp_a1 = arg0 + var_s0;
        ((CLoaderEntry *)temp_a1)->inUse = 0;
        CList__AddTail(arg0 + 0x5068, temp_a1);
        var_s1 += 1;
        var_s0 += 0x20;
    } while (var_s1 < 0x210);
    memset(arg0 + 0xA68, arg1 & 0xFF, 0x400);
    osCreateThread(arg0, arg1, &CLoader__Main, arg0, arg0 + 0xE68, D_800B6E94);
    osStartThread(arg0);
}
