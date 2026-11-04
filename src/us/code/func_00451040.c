#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x2B];
    /* 0x2B */ s8 unk2B;
} Settings;

typedef struct {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ Settings *settings;
} Menu;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ Menu *menu;
} MenuCaller;

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

s32 func_00451040(s32 arg0, MenuCaller *arg1) {
    Settings *temp_s0;

    temp_s0 = arg1->menu->settings;
    temp_s0->unk2B = func_0041648C((s32) arg1, (s32) temp_s0->unk2B, 1, 0, 1, 1);
    return 0;
}
