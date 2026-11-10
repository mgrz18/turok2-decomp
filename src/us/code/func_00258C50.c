#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"
#include "structs/weapon.h"
#include "protos/ai.h"

s32 CPlayer__GetWeaponAmmo(sptr, s32);
s32 WeaponActor_CheckFire(sptr, sptr);


void WeaponMode_Fire_Update(CGameObjectInstance *obj, CAI *ai) {
    s16 temp_a1;
    CPlayer *temp_s1;

    temp_s1 = (CPlayer *) obj->m_AI.m_pParent;
    if (ai->m_AnimRepeatedCount != 0) {
        if ((*(f32 *) &ai->m_VarBuffer[3] <= 0.0f) && (temp_a1 = temp_s1->m_Data.m_CurrentWeapon, ((gWeaponInfoTable[temp_a1]->flags & 4) != 0)) && (CPlayer__GetWeaponAmmo((sptr) temp_s1, (s32) temp_a1) != 0) && (WeaponActor_CheckFire((sptr) obj, (sptr) ai) != 0)) {
            if ((temp_s1->m_ContState.m_dwButtons & 0x2000) && (temp_s1->m_pOptions->m_TagMode == 0)) {
                ai->m_AnimRepeatedCount = 0;
                ai->m_ModeLastFrame = -1;
                ai->m_Mode = -1;
                CAI__SetMode(obj, ai, 4);
                *(f32 *) &ai->m_VarBuffer[3] = (f32) (gWeaponInfoTable[((CPlayer *) obj->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * 15.0f);
            } else {
                goto block_9;
            }
        } else {
block_9:
            CAI__SetMode(obj, ai, 2);
        }
        temp_s1->m_CrossHairMode = 1;
        temp_s1->m_CrossHairTimer = 0;
    }
}
