#include "common.h"
#include "structs/player.h"

s32 func_00248720(s32, s32);
s32 func_00253DE0(s32, s32);
s32 func_00253EB8(s32, s32);

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 unk18;
} WeaponDef;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 scale;
} Tuning;

extern Tuning D_800A72C8;
extern WeaponDef *D_800AF618[];

void func_00259040(CGameObjectInstance *obj, CAI *ai) {
    s32 temp_s1;

    temp_s1 = (s32) obj->m_AI.m_pParent;
    ai->m_ConditionTimer = 0.0f;
    func_00248720((s32) obj->m_AI.m_pParent, 0x12B);
    func_00253DE0(temp_s1, 0x1A5);
    func_00253EB8(temp_s1, 0x1A7);
    *(f32 *) &ai->m_VarBuffer[3] = D_800AF618[((CPlayer *) obj->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->unk18 * D_800A72C8.scale;
}
