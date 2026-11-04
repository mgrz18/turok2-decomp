#include "common.h"
#include "structs/player.h"

/* The weapon AI's own variables, in its m_VarBuffer. */
#define ANGLE(ai) (*(f32 *) &(ai)->m_VarBuffer[0])
#define TILT(ai) (*(f32 *) &(ai)->m_VarBuffer[1])

extern f32 func_00211774(f32 value, f32 target, f32 rate);
extern void func_00253DE0(CPlayer *player, s32 sound);
extern void func_00253E68(CPlayer *player);

void func_002593D8(void *obj, CAI *state) {
    CPlayer *player = *(CPlayer **)((u8 *)obj + 0x1A8);
    f32 target;
    f32 rate;
    f32 dir;
    f32 tilt;

    if (player->m_ContState.m_FinalMoveY > 0.0f && state->m_Mode != 1) {
        target = -0.41887906f;
        if (player->m_Speed > 0.0f) {
            target = 0.41887906f;
        }
        dir = 1.0f;
        rate = player->m_TotalSpeed * 0.0525f;
        if (player->m_Speed < 0.0f) {
            dir = -1.0f;
        }
        if (TILT(state) < 0.0f ? dir != -1.0f : dir != 1.0f) {
            rate *= 4.0f;
        }
        if (player->m_pLoopingSound_WeaponBackground == 0) {
            func_00253DE0(player, 0x467);
        }
    } else {
        rate = 0.008726647124f;
        target = -0.017453294f;
        if (TILT(state) > 0.0f) {
            target = 0.017453294f;
        }
        if (player->m_pLoopingSound_WeaponBackground != 0) {
            func_00253E68(player);
        }
    }
    tilt = func_00211774(TILT(state), target, rate);
    TILT(state) = tilt;
    ANGLE(state) -= tilt;
}
