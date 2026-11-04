#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"

/* The weapon AI's own variables, in its m_VarBuffer. */
#define SPIN(ai) (*(f32 *) &(ai)->m_VarBuffer[1])

/* Weapon state: winding a spun-up weapon (barrel spin) while fire is held, else stopping it. */


extern f32 D_800B6D28; /* frame time scale */

extern s32 func_0024E700(CPlayer *player, s32 weapon);
extern s32 func_00243414(void *obj, CAI *state, s32 mode);
extern s32 func_00275544(s32 sound, CVector3 position, CVector3 *source, s32 arg5);
extern void func_00253F40(CPlayer *player);
extern void func_00253E68(CPlayer *player);
extern s32 func_00253EB8(CPlayer *player, s32 sound);
extern s32 func_00248720(void *player, s32 arg1);
extern void func_002117DC(f32 *value, f32 target, f32 rate);

void func_00258370(void *obj, CAI *state) {
    CPlayer *player = *(CPlayer **)((u8 *)obj + 0x1A8);
    CVector3 *pos = (CVector3 *)(((u8 *)player->m_pCamera) + 0x114);

    if (!(player->m_ContState.m_dwButtons & 0x2000) || player->m_bHeadMovementDisabled != 0
        || func_0024E700(player, player->m_Data.m_CurrentWeapon) == 0) {
        func_00243414(obj, state, 2);
        if (player->m_pLoopingSound_WeaponFiring != 0) {
            func_00275544(0x1B5, *pos, pos, -1);
        }
        func_00253F40(player);
        func_00253E68(player);
        func_00275544(0x1A6, *pos, pos, -1);
        return;
    }
    func_002117DC(&SPIN(state), 0.3490659f, 0.2f);
    if (SPIN(state) < 0.2617994249f) {
        return;
    }
    if (player->m_pLoopingSound_WeaponFiring == 0) {
        func_00253EB8(player, 0x1A7);
    }
    state->m_ConditionTimer -= D_800B6D28;
    if (state->m_ConditionTimer <= 0.0f) {
        state->m_ConditionTimer += 1.875f;
        func_00248720(*(void **)((u8 *)obj + 0x1A8), 0x1A);
    }
}
