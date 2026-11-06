#include "common.h"
#include "structs/player.h"

extern u8 D_8011AAD8[];   /* input state */
extern f32 gFrameIncrement;    /* frame time scale */

/* Crouch: toggled by button 0x400 while standing or walking (poses 2 and 3),
 * forced by flags 0x6000; eases the crouch height toward 40.96 or 0 and sets
 * bit 0x80 of *out while it is within 10.24 of fully crouched. */
void CPlayer__UpdateCrouch(CPlayer *player, s32 unused, s32 *flags, s32 *out) {
    s32 crouching;
    f32 delta;
    u8 *input;

    crouching = 0;
    if (*flags & 0x6000) {
        crouching = 1;
    } else if ((u16)(player->m_Mode - 2) < 2) {
        if (player->m_ContState.m_dwButtonsSingle & 0x400) {
            input = D_8011AAD8;
            if (input[0x19] == 0) {
                if (*(s32 *)(input - 0x38) == 0 || !(player->m_ContState.m_dwButtons & 0x20)) {
                    player->m_bDuckMode ^= 1;
                }
            }
        }
    } else {
        player->m_bDuckMode = 0;
    }
    if (player->m_bDuckMode != 0) {
        crouching = 1;
    }
    if (crouching) {
        delta = 0.25f * (40.96f - player->m_DuckOffset);
    } else {
        delta = -player->m_DuckOffset * 0.25f;
    }
    player->m_DuckOffset += delta * gFrameIncrement;
    delta = 40.96f - player->m_DuckOffset;
    if (-10.24f <= delta && delta <= 10.24f) {
        *out |= 0x80;
    } else {
        *out &= ~0x80;
    }
}
