#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

s32 func_00253DE0(s32, s32);

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 scale;
} Tuning;

extern Tuning D_800A72A0;

void func_00258E38(CGameObjectInstance *obj, CAI *ai) {
    s32 player;

    player = (s32) obj->m_AI.m_pParent;
    ai->m_ConditionTimer = 0.0f;
    func_00253DE0(player, 0x1A5);
    *(f32 *) &ai->m_VarBuffer[3] = D_800AF618[((CPlayer *) obj->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * D_800A72A0.scale;
}
