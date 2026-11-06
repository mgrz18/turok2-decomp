#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"
#include "structs/weapon.h"

s32 CAI__SetMode(s32, s32, s32);
s32 CPlayer__GetWeaponAmmo(s32, s32);
s32 WeaponActor_CheckFire(s32, s32);

extern f32 D_800A7298;

void WeaponMode_Fire_Update(CGameObjectInstance *obj, CAI *ai) {
    s16 temp_a1;
    CPlayer *temp_s1;

    temp_s1 = (CPlayer *) obj->m_AI.m_pParent;
    if (ai->m_AnimRepeatedCount != 0) {
        if ((*(f32 *) &ai->m_VarBuffer[3] <= 0.0f) && (temp_a1 = temp_s1->m_Data.m_CurrentWeapon, ((D_800AF618[temp_a1]->flags & 4) != 0)) && (CPlayer__GetWeaponAmmo((s32) temp_s1, (s32) temp_a1) != 0) && (WeaponActor_CheckFire((s32) obj, (s32) ai) != 0)) {
            if ((temp_s1->m_ContState.m_dwButtons & 0x2000) && (temp_s1->m_pOptions->m_TagMode == 0)) {
                ai->m_AnimRepeatedCount = 0;
                ai->m_ModeLastFrame = -1;
                ai->m_Mode = -1;
                CAI__SetMode((s32) obj, (s32) ai, 4);
                *(f32 *) &ai->m_VarBuffer[3] = (f32) (D_800AF618[((CPlayer *) obj->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * D_800A7298);
            } else {
                goto block_9;
            }
        } else {
block_9:
            CAI__SetMode((s32) obj, (s32) ai, 2);
        }
        temp_s1->m_CrossHairMode = 1;
        temp_s1->m_CrossHairTimer = 0;
    }
}
