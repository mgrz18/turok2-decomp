#include "common.h"
#include "structs/game_object.h"
#include "globals/boss.h"

void Primagen_EnterMode23(CGameObjectInstance *obj) {
    f32 speed = 1.0f;

    obj->m_AI.m_AnimSpeed = speed;
    gLevelScript.timer = speed;
    obj->m_dwFlags |= 0x2000;
}
