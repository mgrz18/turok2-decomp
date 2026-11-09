#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "globals/engine.h"
#include "protos/ai.h"

/* Weapon state: spins the barrel up while fire is held, else stops it. */



extern s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);
extern s32 Sound__Play(s32 sound, CVector3 position, CVector3 *source, s32 arg5);
extern void CPlayer__StopWeaponFiringSound(CPlayer *player);
extern void CPlayer__StopWeaponBackgroundSound(CPlayer *player);
extern s32 CPlayer__StartWeaponFiringSound(CPlayer *player, s32 sound);
extern s32 CPlayer__FireWeapon(void *player, s32 arg1);
extern void Math_EaseToward(f32 *value, f32 target, f32 rate);

void WeaponMode_SpinUpFire_Update(void *obj, CAI *state) {
    CPlayer *player = *(CPlayer **)((u8 *)obj + 0x1A8);
    CVector3 *pos = (CVector3 *)(((u8 *)player->m_pCamera) + 0x114);

    if (!(player->m_ContState.m_dwButtons & 0x2000) || player->m_bHeadMovementDisabled != 0
        || CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon) == 0) {
        CAI__SetMode(obj, state, 2);
        CPlayer__StopWeaponBackgroundSound(player);
        CPlayer__StopWeaponFiringSound(player);
        Sound__Play(0x1A6, *pos, pos, -1);
        return;
    }
    if (player->m_pLoopingSound_WeaponFiring == 0) {
        CPlayer__StartWeaponFiringSound(player, 0x1A7);
    }
    Math_EaseToward(&state->m_Weapon.m_BarrelSpeed, 0.2792527f, 0.4f);
    state->m_ConditionTimer -= gFrameIncrement;
    if (state->m_ConditionTimer <= 0.0f) {
        state->m_ConditionTimer += 1.875f;
        CPlayer__FireWeapon(*(void **)((u8 *)obj + 0x1A8), 0x12B);
    }
}
