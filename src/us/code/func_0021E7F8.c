#include "common.h"
#include "structs/game_object.h"

extern s32 func_0020C0D0(s32, s32, s32);

void func_0021E7F8(CGameObjectInstance *obj, s32 arg1) {
    ((void (*)(void *, void *)) obj->m_AI.m_pfCalculateOrientationMatrix)(obj, &obj->m_AI);
    func_0020C0D0((s32) obj->m_mfOrient, arg1, (s32) &obj->m_vMin);
}
