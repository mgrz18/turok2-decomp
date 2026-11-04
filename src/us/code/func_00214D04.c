#include "common.h"

/* The record this spawns from: an id at 0x24, extra spawn flags at 0x80 and its own flags at 0x88 (0x40: disabled). */
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 id;
    /* 0x28 */ u8 pad28[0x80 - 0x28];
    /* 0x80 */ s32 spawnFlags;
    /* 0x84 */ u8 pad84[4];
    /* 0x88 */ u16 flags;
} SpawnSource;

s32 func_00203CA0(s32, s32, s32, s32, s32, void *, s32 *, s32 *, s32); /* extern */
s32 func_002051F4(s32, s32);                /* extern */
s32 func_0020BE08(s32);                         /* extern */
s32 func_00214DBC();                                /* extern */
extern s32 D_800A57F8;
extern s32 D_800ACAD8;
extern s32 func_00214F14;

void func_00214D04(SpawnSource *arg0) {
    s32 temp_v0;
    s32 temp_v0_2;

    if (!(arg0->flags & 0x40)) {
        temp_v0 = func_00214DBC();
        if (temp_v0 != 0) {
            func_0020BE08(temp_v0);
            temp_v0_2 = func_00203CA0(0, arg0->spawnFlags | D_800ACAD8, temp_v0, arg0->id, 8, arg0, &func_00214F14, &D_800A57F8, 0);
            if (temp_v0_2 != 0) {
                func_002051F4(0, temp_v0_2);
            }
            func_002051F4(0, temp_v0);
        }
    }
}
