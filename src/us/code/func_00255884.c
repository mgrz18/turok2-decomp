#include "common.h"
#include "structs/game_object.h"

/* The player, as read here (CPlayer's header from 0xA08 on does not line up
 * with these offsets: see func_00250430.c, func_00255B50.c). */
typedef struct {
    /* 0x000 */ u8 pad0[0xA10];
    /* 0xA10 */ f32 modeTime;
    /* 0xA14 */ u8 padA14[0xA5C - 0xA14];
    /* 0xA5C */ f32 strafe;
    /* 0xA60 */ u8 padA60[0xA7C - 0xA60];
    /* 0xA7C */ f32 unkA7C;
} Player;

s32 func_0024E5F0(s32, s32, s32);
f32 func_00255940(s32, s32);

void func_00255884(Player *arg0, CGameObjectInstance *arg1) {
    f32 temp_f2;

    temp_f2 = arg0->unkA7C;
    if (((temp_f2 < 0.0f) && (arg0->strafe >= 0.0f)) || ((temp_f2 > 0.0f) && (arg0->strafe <= 0.0f)) || (arg0->modeTime >= 1.5f)) {
        func_0024E5F0((s32) arg0, (s32) arg1, 7);
    } else {
        arg1->m_vVelocity.y = 307.19998f;
    }
    func_00255940((s32) arg0, (s32) arg1);
}
