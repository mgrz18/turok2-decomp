#include "common.h"

typedef struct {
    /* 0x00000 */ u8 pad0[0x18FA8];
    /* 0x18FA8 */ s32 group;
    /* 0x18FAC */ s32 warpMode;
    /* 0x18FB0 */ s32 warpPending;
    /* 0x18FB4 */ s32 unk18FB4;
    /* 0x18FB8 */ s32 spawnsReady;
    /* 0x18FBC */ u8 pad18FBC[0x18FD0 - 0x18FBC];
    /* 0x18FD0 */ s32 spawnKey;
    /* 0x18FD4 */ s32 warpID;
    /* 0x18FD8 */ s32 unk18FD8;
    /* 0x18FDC */ s32 unk18FDC;
} World;

s32 func_0042C700(s32, s32, s32);

void func_0042C4B8(World *arg0, s32 arg1) {
    if (arg1 != arg0->group) {
        arg0->spawnsReady = 0;
        func_0042C700((s32) arg0, ~arg1, 0);
    }
}
