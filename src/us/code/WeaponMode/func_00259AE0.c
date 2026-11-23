#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


#ifdef TARGET_PORT
#include "globals/engine.h"
/* Host body: the State view is the CAI (m_Mode 0x34, m_AnimType 0xC4, the weapon vars
 * 0x10C/0x110/0x128/0x12C); the player's ammo by member (N64 0x934 = m_Data.m_AmmoList). */
f32 Math_Approach(f32 value, f32 target, f32 rate);
s16 *CPlayer__GetFiringAmmo(CPlayer *player, s32 id);

void WeaponBarrel_Spin_AF180(CGameObjectInstance *actor, CAI *state) {
    f32 v;

    v = Math_Approach(state->m_Weapon.m_BarrelSpeed, 0.174532949924469f /* 10 deg */, 0.0175f);
    state->m_Weapon.m_BarrelSpeed = v;
    state->m_Weapon.m_BarrelAngle += v * gFrameIncrement;
    if (state->m_Mode == 4 && state->m_AnimType != 0x4E84) {
        if (7.0f <= actor->m_CurrentAnimation.m_AnimationFrame) {
            if (state->m_Weapon.m_nLod == 0) {
                state->m_Weapon.m_nPass = 0;
            }
            state->m_Weapon.m_nLod = 1;
        } else {
            state->m_Weapon.m_nLod = 0;
        }
    } else {
        CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

        if (player->m_Data.m_AmmoList[*CPlayer__GetFiringAmmo(player, 0x15)] != 0) {
            state->m_Weapon.m_nLod = 0;
        } else {
            state->m_Weapon.m_nLod = 1;
        }
    }
}
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ s8 unk34;
    /* 0x035 */ u8 pad35[0xC4 - 0x35];
    /* 0x0C4 */ s16 unkC4;
    /* 0x0C6 */ u8 padC6[0x10C - 0xC6];
    /* 0x10C */ f32 unk10C;
    /* 0x110 */ f32 unk110;
    /* 0x114 */ u8 pad114[0x128 - 0x114];
    /* 0x128 */ s32 unk128;
    /* 0x12C */ s32 unk12C;
} State;

/* Frame globals; the time step (gFrameIncrement) must be read through the block to match. */
extern struct {
    /* 0x0 */ u32 frame;
    /* 0x4 */ u32 pad4;
    /* 0x8 */ f32 delta;
} gFrameCountGameplay;
f32 Math_Approach(f32 value, f32 target, f32 rate);
s16 *CPlayer__GetFiringAmmo(CPlayer *player, s32 id);

void WeaponBarrel_Spin_AF180(CGameObjectInstance *actor, State *state) {
    f32 v;

    v = Math_Approach(state->unk110, 0.174532949924469f /* 10 deg */, 0.0175f);
    state->unk110 = v;
    state->unk10C += v * gFrameCountGameplay.delta;
    if (state->unk34 == 4 && state->unkC4 != 0x4E84) {
        if (7.0f <= actor->m_CurrentAnimation.m_AnimationFrame) {
            if (state->unk128 == 0) {
                state->unk12C = 0;
            }
            state->unk128 = 1;
        } else {
            state->unk128 = 0;
        }
    } else {
        CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

        if (*(s16 *) ((s8 *) player + *CPlayer__GetFiringAmmo(player, 0x15) * 2 + 0x934) != 0) {
            state->unk128 = 0;
        } else {
            state->unk128 = 1;
        }
    }
}
#endif
