#include "common.h"
#include "structs/camera.h"
#include "structs/player.h"
#include "structs/weapon.h"

s32 Sound__Play(s16, CVector3, s32, s32);

void WeaponMode_AE7F4_Enter(CGameObjectInstance *obj, CAI *ai) {
    CVector3 *temp_v0;
    CCamera *temp_v1;

    temp_v1 = ((CPlayer *) obj->m_AI.m_pParent)->m_pCamera;
    temp_v0 = &temp_v1->m_vPos;
    Sound__Play(0x1BB, *temp_v0, (sptr) temp_v0, -1);
    ai->m_ModeTime = 0.0f;
    *(f32 *) &ai->m_VarBuffer[3] = (f32) (gWeaponInfoTable[((CPlayer *) obj->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * 15.0f);
}
