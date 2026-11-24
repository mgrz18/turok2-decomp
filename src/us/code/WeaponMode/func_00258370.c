#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "globals/engine.h"
#include "protos/ai.h"
#include "protos/weapon_mode.h"
#ifdef TARGET_PORT
#include "structs/camera.h"
#endif

/* Weapon state: winding a spun-up weapon (barrel spin) while fire is held, else stopping it. */



extern void CPlayer__StopWeaponFiringSound(CPlayer *player);
extern void CPlayer__StopWeaponBackgroundSound(CPlayer *player);
extern void Math_EaseToward(f32 *value, f32 target, f32 rate);

void WeaponMode_SpinFire_Update(void *obj, CAI *state) {
    CPlayer *player = (*(CPlayer **)&((CGameObjectInstance *)obj)->m_AI.m_pParent);
#ifdef TARGET_PORT
    CVector3 *pos = &player->m_pCamera->m_vPos; /* N64 camera 0x114 */
#else
    CVector3 *pos = (CVector3 *)(((u8 *)player->m_pCamera) + 0x114);
#endif

    if (!(player->m_ContState.m_dwButtons & 0x2000) || player->m_bHeadMovementDisabled != 0
        || CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon) == 0) {
        CAI__SetMode(obj, state, 2);
        if (player->m_pLoopingSound_WeaponFiring != 0) {
            Sound__Play(0x1B5, *pos, pos, -1);
        }
        CPlayer__StopWeaponFiringSound(player);
        CPlayer__StopWeaponBackgroundSound(player);
        Sound__Play(0x1A6, *pos, pos, -1);
        return;
    }
    Math_EaseToward(&state->m_Weapon.m_BarrelSpeed, 0.3490659f, 0.2f);
    if (state->m_Weapon.m_BarrelSpeed < 0.2617994249f) {
        return;
    }
    if (player->m_pLoopingSound_WeaponFiring == 0) {
        CPlayer__StartWeaponFiringSound(player, 0x1A7);
    }
    state->m_ConditionTimer -= gFrameIncrement;
    if (state->m_ConditionTimer <= 0.0f) {
        state->m_ConditionTimer += 1.875f;
        CPlayer__FireWeapon((*(void **)&((CGameObjectInstance *)obj)->m_AI.m_pParent), 0x1A);
    }
}
