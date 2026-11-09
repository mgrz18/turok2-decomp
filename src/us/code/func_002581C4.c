#include "common.h"
#include "structs/player.h"
#include "globals/player.h"
#include "protos/ai.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))


extern s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);

/* Weapon state: sets the fire-rate scale gWeaponChargeScale from the time held for
 * weapons 2-3 (and their 0x1294 ramp) and 12, then goes to state 7 or 5. */
void WeaponMode_WindUp_Update(CGameObjectInstance *obj, CAI *state) {
    CPlayer *player = (CPlayer *) obj->m_AI.m_pParent;
    f32 t;
    f32 scale;

    if (player->m_ContState.m_dwButtons & 0x2000) {
        return;
    }
    switch (player->m_Data.m_CurrentWeapon) {
    case 2:
    case 3:
        if (player->m_SniperMode) {
            gWeaponChargeScale = 1.5f;
        } else {
            scale = MAX(MIN(state->m_ModeTime * (2.0f / 21), 1.5f), 0.5f);
            gWeaponChargeScale = scale;
            t = state->m_ModeTime * (2.0f / 15) + 0.5f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            player->weapon_particle_damage_scaler = t;
        }
        break;
    case 12:
        gWeaponChargeScale = MAX(MIN(state->m_ModeTime * (2.0f / 21), 2.0f), 0.5f);
        break;
    }
    if (CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon) == 1) {
        CAI__SetMode(obj, state, 7);
    } else {
        CAI__SetMode(obj, state, 5);
    }
}
