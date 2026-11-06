#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define P_F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

extern f32 gFrameIncrement; /* the frame's time step */

extern void CPlayer__SetState(void *player, void *other, s32 mode);

void PlayerDeathWait_Update(CPlayer *player) {
    GameMatch *state;
    f32 timer;

    timer = P_F32(player, 0xBFC) + gFrameIncrement;
    P_S32(player, 0xC04) = 1;
    P_S32(player, 0x18) = 0;
    P_S32(player, 0x1C) = 0;
    P_S32(player, 0x20) = 0;
    P_S32(player, 0x1298) = 0;
    P_S32(player, 0xD4) &= ~0x800000;
    P_F32(player, 0xBFC) = timer;
    if (timer > 75.0f || ((state = &D_8011ACB0)->teamOver != 0 && state->over != 0)) {
        state = &D_8011ACB0;
        if (state->teamOver != 0 && state->over == 0) {
            CPlayer__SetState(player, player, 0x11);
        } else {
            CPlayer__SetState(player, player, 0x10);
        }
    }
}
