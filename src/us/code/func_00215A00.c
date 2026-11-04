#include "common.h"

/* Only the field read here: the key of the resource this loads. */
typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ s32 key;
} ResourceOwner;

s32 func_0020185C(s32, s32, s32, s32 *);            /* extern */
s32 *func_0020367C(s32, s32, s32, s32, s32, void *, void *, s32); /* extern */
void func_002051F4(s32, s32 *);                     /* extern */
void func_0020BEE8(s32 *);                          /* extern */
extern u8 D_800A5858[];
extern u8 D_800A5870[];
extern void func_0020B4B0();

void func_00215A00(ResourceOwner *arg0) {
    s32 sp20;
    s32 *temp_v0;
    s32 *var_s0;
    s32 temp_s0;

    temp_v0 = func_0020367C(0, arg0->key, 0x18, 4, 0, NULL, D_800A5858 + 4, 0);
    if (temp_v0 != NULL) {
        temp_s0 = func_0020185C(*temp_v0, arg0->key, 1, &sp20);
        func_002051F4(0, temp_v0);
        var_s0 = func_0020367C(0, temp_s0, sp20, 4, 0, func_0020B4B0, D_800A5870, 0);
    } else {
        var_s0 = NULL;
    }
    if (var_s0 != NULL) {
        func_0020BEE8(var_s0);
        func_002051F4(0, var_s0);
    }
}
