#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

void Primagen_EnterMode20(CGameObjectInstance *obj) {
    obj->m_dwFlags |= 0x2000;
    D_8012F9DC.swayTimer = 15.0f;
    D_8012F9DC.timer = 11.25f;
}
