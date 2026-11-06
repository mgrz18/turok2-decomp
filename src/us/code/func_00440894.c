#include "common.h"
#include "m2c_macros.h"
#include "structs/game_object.h"

void Primagen_EnterMode27_2(CGameObjectInstance *arg0) {
    arg0->m_AI.m_AnimSpeed = 1.0f;
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2000);
}
