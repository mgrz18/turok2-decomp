#include "common.h"
#include "structs/player.h"
#include "structs/controller.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))


extern s16 D_800AE9D2;

extern f32 Random_Blend(f32 lo, f32 hi); /* random in range */
extern s32 CAI__SetMode(void *obj, CAI *state, s32 mode);

/* Weapon state: on release, charges by the time held (0.1 + t/45, at most 1)
 * and sets the shot count D_800AE9D2 (charge * 3, at least 1), then goes to
 * state 5. */
void WeaponMode_Charge_Update(CGameObjectInstance *obj, CAI *state) {
    f32 base = 0.1f;
    CPlayer *player = (CPlayer *) obj->m_AI.m_pParent;
    f32 charge;
    f32 n;
    f32 t;
    s16 shots;

    player->m_ContState.m_pContState->rumbleBase = Random_Blend(base, 0.3f);
    if (player->m_ContState.m_dwButtons & 0x2000) {
        return;
    }
    t = state->m_ModeTime * (1.0f / 45) + base;
    charge = MIN(t, 1.0f);
    player->weapon_particle_damage_scaler = charge;
    n = charge * 3.0f;
    shots = (n < 1.0f) ? 1 : (s32) n;
    D_800AE9D2 = shots;
    CAI__SetMode(obj, state, 5);
    player->m_ContState.m_pContState->rumbleBase = 0.0f;
}
