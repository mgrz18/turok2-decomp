#include "common.h"

/* D_8011AAE8 (D_8011AAD8 + 0x10, in the multiplayer settings): a halfword setting, whose low byte is copied to 2 unless it is 0xFF or more. */
typedef struct {
    /* 0x00 */ union {
        u16 value;
        u8 bytes[2];
    } setting;
    /* 0x02 */ u8 unk2;
} MenuSetting10;

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

extern MenuSetting10 D_8011AAE8;

s32 func_0041C458(s32 arg0, s32 arg1) {
    u16 temp_v0;
    u8 var_v1;

    temp_v0 = func_0041648C(arg1, D_8011AAE8.setting.value, 0x10, 0, 0x100, 0);
    D_8011AAE8.setting.value = temp_v0;
    var_v1 = 0xFF;
    if ((u32) (temp_v0 & 0xFFFF) < 0xFFU) {
        var_v1 = D_8011AAE8.setting.bytes[1];
    }
    D_8011AAE8.unk2 = var_v1;
    return 0;
}
