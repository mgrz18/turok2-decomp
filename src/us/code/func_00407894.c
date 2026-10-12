#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

extern f32 D_800C054C;

void func_00407894(CGameObjectInstance *arg0) {
    arg0->m_AI.m_AnimSpeed = (f32) D_800C054C;
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
}
