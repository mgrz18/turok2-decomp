#include "common.h"

/* The player, as read here; 0x524 is m_Data.m_Health and 0x1298-0x12AC
 * are CPlayer's m_ShockedTimer, m_pHitMeWithCBore and m_pHitMeWithChargeDart
 * (the state at 0xA08 is as in func_0024E9D0.c). */
typedef struct {
    /* 0x0000 */ u8 pad0[0x524];
    /* 0x0524 */ s32 health;
    /* 0x0528 */ u8 pad528[0xA08 - 0x528];
    /* 0x0A08 */ s16 state;
    /* 0x0A0A */ u8 padA0A[0x1298 - 0xA0A];
    /* 0x1298 */ f32 shockedTimer;
    /* 0x129C */ u8 pad129C[0x12A8 - 0x129C];
    /* 0x12A8 */ s32 hitMeWithCBore;
    /* 0x12AC */ s32 hitMeWithChargeDart;
} Player;

/* The particle that hit the player: its type and its owner. */
typedef struct {
    /* 0x000 */ u8 pad0[2];
    /* 0x002 */ u16 type;
    /* 0x004 */ u8 pad4[0x10C - 0x4];
    /* 0x10C */ s32 owner;
} Particle;

s32 func_0024E5F0(s32, s32, s32);
s32 func_002548FC(s32);

void func_00253770(Player *arg0, s32 arg1, s32 arg2, Particle *arg3) {
    f32 temp_f0;
    f32 temp_f1;
    f32 var_f0;
    u16 temp_v1;

    if ((arg0->health != 0) && (func_002548FC((s32) arg0) == 0)) {
        temp_v1 = arg3->type;
        switch (temp_v1) {                          /* irregular */
        case 0x128:
            if (arg0->state != 0x23) {
                temp_f1 = arg0->shockedTimer;
                if (temp_f1 > 0.0f) {
                    temp_f0 = temp_f1 + 30.0f;
                    if (!(temp_f0 <= 150.0f)) {
                        var_f0 = 150.0f;
                        goto block_12;
                    }
                    arg0->shockedTimer = temp_f0;
                } else {
                    var_f0 = 60.0f;
block_12:
                    arg0->shockedTimer = var_f0;
                }
                arg0->hitMeWithChargeDart = (s32) arg3->owner;
                return;
            }
            break;
        case 0x129:
            if (arg0->hitMeWithCBore == 0) {
                arg0->hitMeWithCBore = (s32) arg3->owner;
            }
            arg0->shockedTimer = 0.0f;
            func_0024E5F0((s32) arg0, (s32) arg0, 0x23);
            break;
        }
    }
}
