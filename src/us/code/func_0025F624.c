#include "common.h"
#include "rw/abu_protos.h"

/* The AI fields this mode callback touches; 0x10C-0x13B is CAI::m_VarBuffer,
 * the per-mode scratch. */
typedef struct {
    /* 0x000 */ u8 pad0[0x134];
    /* 0x134 */ f32 timer;
    /* 0x138 */ f32 timer2;
} AI;

f32 func_002119FC(f32 arg0, f32 arg1);

extern f32 D_800A790C;
extern f32 D_800A7910;

void func_0025F624(s32 arg0, AI *arg1) {
    f32 temp_f20;
    f32 temp_f21;

    temp_f20 = D_800A790C;
    temp_f21 = D_800A7910;
    arg1->timer = func_002119FC(temp_f20, temp_f21);
    arg1->timer2 = func_002119FC(temp_f20, temp_f21);
}
