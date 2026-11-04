#include "common.h"
#include "structs/game.h"

/* The game state (CEngineApp in func_00286440). */

void func_0029AA80(s32);
s32 func_0029AAD0(s32);

s32 func_0029AC40(f32);                         /* extern */
extern f32 D_800A9E24;
extern s32 D_800B7BE0;
extern s32 D_800B7E60;
extern s32 D_800C2038;

void func_00289D94(CEngineApp *arg0) {
    s32 *var_a0;

    if (arg0->m_Settings.unk1C8 != 0) {
        var_a0 = &D_800B7E60;
        if (D_800C2038 == 0) {
            var_a0 = &D_800B7BE0;
        }
        func_0029AA80((s32) var_a0);
        func_0029AC40(D_800A9E24);
        func_0029AAD0(2);
    }
}
