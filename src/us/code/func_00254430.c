#include "common.h"

s32 func_00249358(s32, s32, s32, s32, s32);

s32 func_0027580C(s32);                     /* extern */
extern f32 D_800B6D28;
extern u8 D_8011AAF1;

/* Player air timer: warns at 300, then every 600 hurts the player (more when healthy) and backs off 37.5. */
typedef struct {
    /* 0x000 */ u8 pad0[0x524];
    /* 0x524 */ s32 health;      /* CPlayer.m_Data.m_Health, 24.8 fixed point */
    /* 0x528 */ u8 pad528[0xBE4 - 0x528];
    /* 0xBE4 */ f32 airTimer;
    /* 0xBE8 */ u8 padBE8[4];
    /* 0xBEC */ s32 outOfAir;
} PlayerAir;

void func_00254430(PlayerAir *arg0, s32 arg1) {
    f32 temp_f0;
    f32 temp_f1;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s1;

    temp_f1 = arg0->airTimer;
    temp_f0 = temp_f1 + D_800B6D28;
    arg0->airTimer = temp_f0;
    if ((temp_f1 < 300.0f) && (temp_f0 > 300.0f) && (D_8011AAF1 == 0)) {
        func_0027580C(0x163);
    }
    if (arg0->airTimer > 600.0f) {
        temp_v0 = (s32) arg0->health >> 8;
        var_s1 = 1;
        if ((0x96 - temp_v0) > 0) {
            temp_v1 = (0x96 - temp_v0) / 25;
            if (temp_v1 > 0) {
                var_s1 = temp_v1;
            }
        }
        if (arg0->outOfAir == 0) {
            if (D_8011AAF1 == 0) {
                func_0027580C(0x164);
            }
        }
        arg0->outOfAir = 1;
        func_00249358((s32) arg0, arg1, var_s1 << 8, 0, 0x1C);
        arg0->airTimer = (f32) (arg0->airTimer - 37.5f);
    }
}
