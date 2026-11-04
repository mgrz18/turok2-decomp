#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

extern f32 D_800A7364;

void func_00259E30(s32 arg0, CAI *ai) {
    ai->m_Weapon.m_RefireDelay = D_800A7364;
    func_00243414(arg0, (s32) ai, 4);
}
