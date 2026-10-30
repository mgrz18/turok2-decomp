#include "common.h"

s32 func_004160F0(void *item);
s32 func_0029E3E0(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800C1418[];

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ f32 seconds;
    /* 0x10 */ f32 minutes;
    /* 0x14 */ f32 hours;
} PlayTime;

extern PlayTime *D_800C201C;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItem;

s32 func_00422060(MenuItem *item) {
    s32 seconds = D_800C201C->seconds;
    s32 minutes = D_800C201C->minutes;
    s32 hours = D_800C201C->hours;

    func_0029E3E0(item->text + (func_004160F0(item) - 9), D_800C1418, hours, minutes, seconds);
    return 1;
}
