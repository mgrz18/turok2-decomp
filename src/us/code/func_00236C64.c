#include "structs/particle.h"

typedef struct Emitter {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ CParticle *owner;
    /* 0x20 */ u8 pad20[0x24 - 0x20];
    /* 0x24 */ f32 unk24;
} Emitter;

s32 func_0020D908(s32, s32);
s32 func_0020F7EC(s32);
s32 func_002333C0(s32, s32, s32);
s32 func_0021034C(u8 *, f32, f32, f32);    /* extern */

extern f32 D_800A653C;
extern f32 D_800A6540;

void func_00236C64(s32 arg0, Emitter *arg1, CParticle *arg2, s32 arg3) {
    u8 sp10[0x40];
    f32 var_f0;
    CParticle *temp_s0;

    temp_s0 = arg1->owner;
    if ((temp_s0 == arg2) && (arg1->unk24 > 0.0f)) {
        func_0020D908((s32) sp10, arg3);
        var_f0 = D_800A653C;
        if (temp_s0->m_pEffect->m_pModel != NULL) {
            var_f0 = D_800A6540;
        }
        func_0021034C(sp10, var_f0, var_f0, var_f0);
        func_0020F7EC((s32) sp10);
        func_002333C0(arg0, (s32) arg1, (s32) sp10);
    }
}
