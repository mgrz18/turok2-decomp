#include "common.h"

extern s32 CList__MoveToHead(s32, s32);
extern s32 D_800D8DF0[];

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 unk10;
} Unk002065CC;

void CCache__TouchMemEntry(s32 arg0, void *arg1) {
    ((Unk002065CC *)arg1)->unk10 = D_800D8DF0[0];
    CList__MoveToHead((s32)((s8 *)D_800D8DF0 - 0xC10), (s32) arg1);
}
