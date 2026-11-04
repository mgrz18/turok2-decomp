#include "common.h"

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s8 slot;
} MenuItem;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ MenuItem *item;
} MenuCaller;

s32 func_00285A68(s8);                          /* extern */
extern s32 D_800C2020;

void func_004524F4(MenuCaller *arg0) {
    func_00285A68(arg0->item->slot);
    D_800C2020 = 0;
}
