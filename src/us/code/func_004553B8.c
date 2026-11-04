#include "common.h"

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

/* A 16-bit menu value at D_8011AAE8 and its 8-bit clamped copy. */
typedef struct {
    union {
        /* 0x0 */ u16 full;
        /* 0x0 */ u8 bytes[2];
    } value;
    /* 0x2 */ u8 clamped;
} Unk8011AAE8;
extern Unk8011AAE8 D_8011AAE8;

s32 func_004553B8(s32 arg0, s32 arg1) {
    u16 temp_v0;
    u8 var_v1;

    temp_v0 = func_0041648C(arg1, (s32) D_8011AAE8.value.full, 0x10, 0, 0x100, 0);
    D_8011AAE8.value.full = temp_v0;
    var_v1 = 0xFF;
    if ((u32) (temp_v0 & 0xFFFF) < 0xFFU) {
        var_v1 = D_8011AAE8.value.bytes[1];
    }
    D_8011AAE8.clamped = var_v1;
    return 0;
}
