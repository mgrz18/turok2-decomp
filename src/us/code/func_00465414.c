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

void func_00465414(World *arg0) {
    arg0->warpMode = 2;
    arg0->warpPending = 0;
    arg0->unk18FB4 = 0;
    arg0->spawnsReady = 0;
    arg0->spawnKey = 0;
    arg0->warpID = 0;
    arg0->unk18FD8 = 0;
    arg0->unk18FDC = 0;
}
