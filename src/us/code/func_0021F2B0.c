#include "common.h"
#include "structs/game_object.h"

/* The first section of an object model (func_002017D4(*model, 0)): only its angle at 0x1E is read. */
typedef struct {
    /* 0x00 */ u8 pad0[0x1E];
    /* 0x1E */ u16 angle;
} ModelSection;

void *func_002017D4(s32, s32);                  /* extern */
s32 *func_0020367C(s32, s32, s32, s32, s32, s32, s32 *, s32); /* extern */
s32 func_002051F4(s32, s32 *);              /* extern */
f32 func_0026D210(u16);                             /* extern */
extern s32 D_800A5AC4;
extern f32 D_800A5D18;

f32 func_0021F2B0(CGameObjectInstance *arg0) {
    f32 var_f0;
    f32 var_f20;
    s32 *temp_v0;

    var_f20 = D_800A5D18;
    if (arg0->m_dwFlags & 0x40000) {
        temp_v0 = func_0020367C(0, (s32) arg0->m_rpObject, arg0->m_rpObjectHeader, 4, 0, 0, &D_800A5AC4, 1);
        var_f0 = var_f20;
        if (temp_v0 != NULL) {
            var_f20 = func_0026D210(((ModelSection *) func_002017D4(*temp_v0, 0))->angle);
            func_002051F4(0, temp_v0);
            goto block_3;
        }
    } else {
block_3:
        var_f0 = var_f20;
    }
    return var_f0;
}
