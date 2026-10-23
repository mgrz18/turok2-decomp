#include "common.h"
#include "structs/game_object.h"

void func_00246918(CGameObjectInstance *obj, CAI *ai);
void func_002280B0(CGameObjectInstance *obj);

void func_0025E9BC(CGameObjectInstance *obj, CAI *ai) {
    f32 t;
    f32 squash;

    ai->m_OnFireTimer = 0;
    obj->m_dwHotPointFlags &= ~2;
    func_00246918(obj, ai);
    t = obj->m_AI.m_ModeTime;
    if (t > 30.0f) {
        squash = 0.01f;
        obj->m_dwFlags &= ~0x100;
        obj->m_dwFlags &= ~0x2000;
        func_00246918(obj, ai);
        if (obj->m_dwFlags & 0x80000) {
            func_002280B0(obj);
        }
    } else {
        squash = t * (0.01f / 30.0f);
    }
    obj->m_vScale.x *= 1.0f + squash;
    obj->m_vScale.y *= 1.0f - squash;
    obj->m_vScale.z *= 1.0f + squash;
}
